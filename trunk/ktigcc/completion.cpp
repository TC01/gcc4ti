/*
   ktigcc - TIGCC IDE for KDE

   Copyright (C) 2006 Kevin Kofler

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
*/

#include <qstring.h>
#include <qvaluelist.h>
#include <qpair.h>
#include <qpoint.h>
#include <qregexp.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <kmessagebox.h>
#include <kate/view.h>
#include <kate/document.h>
#include <ktexteditor/editinterfaceext.h>
#include <kconfig.h>
#include "completion.h"
#include "parsing.h"
#include "preferences.h"
#include "mainform.h"
#include "tpr.h"

// Maps file name to a CompletionInfo.
QMap<QString,CompletionInfo> systemHeaderCompletion, projectCompletion;

static void resetSearchedFlags(void)
{
  for (QMap<QString,CompletionInfo>::Iterator it=projectCompletion.begin();
       it!=projectCompletion.end(); ++it)
    (*it).searched=false;
  for (QMap<QString,CompletionInfo>::Iterator it=systemHeaderCompletion.begin();
       it!=systemHeaderCompletion.end(); ++it)
    (*it).searched=false;
}

static void findSymbolInSystemHeaders(const QString &symbol,
                                      const QStringList &systemHeaders,
                                      QString &symbolFile,
                                      unsigned &symbolLine,
                                      bool &systemHeader)
{
  for (QStringList::ConstIterator it=systemHeaders.begin();
       it!=systemHeaders.end(); ++it) {
    const QString &headerName=*it;
    // Avoid infinite recursion.
    if (systemHeaderCompletion.contains(headerName)
        && !systemHeaderCompletion[headerName].searched) {
      CompletionInfo &completionInfo=systemHeaderCompletion[headerName];
      completionInfo.searched=true;
      if (completionInfo.lineNumbers.contains(symbol)) {
        symbolFile=headerName;
        symbolLine=completionInfo.lineNumbers[symbol];
        systemHeader=true;
        return;
      } else {
        findSymbolInSystemHeaders(symbol,completionInfo.includedSystem,
                                  symbolFile,symbolLine,systemHeader);
        if (!symbolFile.isNull()) return;
      }
    }
  }
}

static bool findSymbolInFileRecursive(const QString &symbol,
                                      const QString &fileText,
                                      const QString &fileName,
                                      MainForm *mainForm,
                                      QString &symbolFile,
                                      unsigned &symbolLine,
                                      bool &systemHeader)
{
  symbolFile=QString::null;
  systemHeader=false;
  if (!projectCompletion.contains(fileName) || projectCompletion[fileName].dirty) {
    QFileInfo fileInfo(fileName);
    QString pathInProject=fileInfo.isRelative()?fileInfo.dirPath():".";
    CompletionInfo completionInfo=parseFileCompletion(fileText,pathInProject);
    if (completionInfo.dirty) return false;
    projectCompletion.insert(fileName,completionInfo);
  }
  CompletionInfo &completionInfo=projectCompletion[fileName];
  // Avoid infinite recursion.
  if (completionInfo.searched) return true;
  completionInfo.searched=true;
  if (completionInfo.lineNumbers.contains(symbol)) {
    symbolFile=fileName;
    symbolLine=completionInfo.lineNumbers[symbol];
    return true;
  }
  for (QStringList::ConstIterator it=completionInfo.included.begin();
       it!=completionInfo.included.end(); ++it) {
    const QString &headerName=*it;
    QString headerText=mainForm->textForHeader(headerName);
    if (!headerText.isNull()) {
      if (!findSymbolInFile(symbol,headerText,headerName,mainForm,symbolFile,
                            symbolLine,systemHeader))
        return false;
      if (!symbolFile.isNull()) return true;
    }
  }
  findSymbolInSystemHeaders(symbol,completionInfo.includedSystem,symbolFile,
                            symbolLine,systemHeader);
  return true;
}

bool findSymbolInFile(const QString &symbol,
                      const QString &fileText,
                      const QString &fileName,
                      MainForm *mainForm,
                      QString &symbolFile,
                      unsigned &symbolLine,
                      bool &systemHeader)
{
  resetSearchedFlags();
  return findSymbolInFileRecursive(symbol,fileText,fileName,mainForm,symbolFile,
                                   symbolLine,systemHeader);
}

static void mergeCompletionEntries(QValueList<KTextEditor::CompletionEntry> &dest,
                                   const QValueList<KTextEditor::CompletionEntry> &src)
{
  for (QValueList<KTextEditor::CompletionEntry>::ConstIterator it=src.begin();
       it!=src.end(); ++it)
    dest.append(*it);
}

static void completionEntriesForSystemHeaders(const QStringList &systemHeaders,
                                              QValueList<KTextEditor::CompletionEntry> &result)
{
  for (QStringList::ConstIterator it=systemHeaders.begin();
       it!=systemHeaders.end(); ++it) {
    const QString &headerName=*it;
    // Avoid infinite recursion.
    if (systemHeaderCompletion.contains(headerName)
        && !systemHeaderCompletion[headerName].searched) {
      CompletionInfo &completionInfo=systemHeaderCompletion[headerName];
      completionInfo.searched=true;
      mergeCompletionEntries(result,completionInfo.entries);
      completionEntriesForSystemHeaders(completionInfo.includedSystem,result);
    }
  }
}

static bool completionEntriesForFileRecursive(const QString &fileText,
                                              const QString &fileName,
                                              MainForm *mainForm,
                                              QValueList<KTextEditor::CompletionEntry> &result)
{
  if (!projectCompletion.contains(fileName) || projectCompletion[fileName].dirty) {
    QFileInfo fileInfo(fileName);
    QString pathInProject=fileInfo.isRelative()?fileInfo.dirPath():".";
    CompletionInfo completionInfo=parseFileCompletion(fileText,pathInProject);
    if (completionInfo.dirty) return false;
    projectCompletion.insert(fileName,completionInfo);
  }
  CompletionInfo &completionInfo=projectCompletion[fileName];
  // Avoid infinite recursion.
  if (completionInfo.searched) return true;
  completionInfo.searched=true;
  mergeCompletionEntries(result,completionInfo.entries);
  completionEntriesForSystemHeaders(completionInfo.includedSystem,result);
  for (QStringList::ConstIterator it=completionInfo.included.begin();
       it!=completionInfo.included.end(); ++it) {
    const QString &headerName=*it;
    QString headerText=mainForm->textForHeader(headerName);
    if (!headerText.isNull())
      if (!completionEntriesForFile(headerText,headerName,mainForm,result))
        return false;
  }
  return true;
}

bool completionEntriesForFile(const QString &fileText,
                              const QString &fileName,
                              MainForm *mainForm,
                              QValueList<KTextEditor::CompletionEntry> &result)
{
  resetSearchedFlags();
  return completionEntriesForFileRecursive(fileText,fileName,mainForm,result);
}

static QValueList<KTextEditor::CompletionEntry> sortCompletionEntries(
  const QValueList<KTextEditor::CompletionEntry> &entries)
{
  QMap<QString,QValueList<KTextEditor::CompletionEntry> > map;
  for (QValueList<KTextEditor::CompletionEntry>::ConstIterator it=entries.begin();
       it!=entries.end(); ++it) {
    const KTextEditor::CompletionEntry &entry=*it;
    map[entry.text].append(entry);
  }
  QValueList<KTextEditor::CompletionEntry> result;
  for (QMap<QString,QValueList<KTextEditor::CompletionEntry> >::ConstIterator
       it=map.begin(); it!=map.end(); ++it)
    mergeCompletionEntries(result,*it);
  return result;
}

bool parseHelpSources(QWidget *parent, const QString &directory,
                      QMap<QString,CompletionInfo> &sysHdrCompletion)
{
  return true; // TODO
}

bool parseSystemHeaders(QWidget *parent, const QString &directory,
                        QMap<QString,CompletionInfo> &sysHdrCompletion)
{
  QDir qdir(directory);
  QStringList headers=qdir.entryList("*.h",QDir::Files);
  for (QStringList::ConstIterator it=headers.begin(); it!=headers.end(); ++it) {
    const QString &header=*it;
    QString fileText=loadFileText(QFileInfo(qdir,header).filePath());
    if (fileText.isNull()) {
      KMessageBox::error(parent,QString("Can't open \'%1\'.").arg(header));
      return false;
    }
    sysHdrCompletion[header]=parseFileCompletion(fileText,QString::null);
    if (sysHdrCompletion[header].dirty) return false;
  }
  return true;
}

void loadSystemHeaderCompletion(void)
{
  KConfig config("ktigcc/completion",true,false,"data");
  QStringList groupList=config.groupList();
  systemHeaderCompletion.clear();
  for (QStringList::ConstIterator it=groupList.begin(); it!=groupList.end(); ++it) {
    const QString &key=*it;
    if (key.endsWith(" Lines")) continue;
    CompletionInfo completionInfo;
    config.setGroup(key);
    completionInfo.includedSystem=config.readListEntry("Included");
    unsigned numEntries=config.readUnsignedNumEntry("Num Entries");
    for (unsigned i=0; i<numEntries; i++) {
      KTextEditor::CompletionEntry entry;
      entry.type=config.readEntry(QString("Entry %1 Type").arg(i));
      entry.text=config.readEntry(QString("Entry %1 Text").arg(i));
      entry.prefix=config.readEntry(QString("Entry %1 Prefix").arg(i));
      entry.postfix=config.readEntry(QString("Entry %1 Postfix").arg(i));
      entry.comment=config.readEntry(QString("Entry %1 Comment").arg(i));
      entry.userdata=config.readEntry(QString("Entry %1 User Data").arg(i));
      completionInfo.entries.append(entry);
    }
    QMap<QString,QString> entryMap=config.entryMap(key+" Lines");
    for (QMap<QString,QString>::ConstIterator it=entryMap.begin();
         it!=entryMap.end(); ++it)
      completionInfo.lineNumbers.insert(it.key(),(*it).toUInt());
    systemHeaderCompletion.insert(key,completionInfo);
  }
}

void saveSystemHeaderCompletion(void)
{
  KConfig config("ktigcc/completion",false,false,"data");
  for (QMap<QString,CompletionInfo>::ConstIterator it=systemHeaderCompletion.begin();
       it!=systemHeaderCompletion.end(); ++it) {
    const QString &key=it.key();
    const CompletionInfo &completionInfo=*it;
    config.setGroup(key);
    config.writeEntry("Included",completionInfo.includedSystem);
    unsigned i=0;
    for (QValueList<KTextEditor::CompletionEntry>::ConstIterator it
         =completionInfo.entries.begin(); it!=completionInfo.entries.end();
         ++it, i++) {
      const KTextEditor::CompletionEntry &entry=*it;
      config.writeEntry(QString("Entry %1 Type").arg(i),entry.type);
      config.writeEntry(QString("Entry %1 Text").arg(i),entry.text);
      config.writeEntry(QString("Entry %1 Prefix").arg(i),entry.prefix);
      config.writeEntry(QString("Entry %1 Postfix").arg(i),entry.postfix);
      config.writeEntry(QString("Entry %1 Comment").arg(i),entry.comment);
      config.writeEntry(QString("Entry %1 User Data").arg(i),entry.userdata);
    }
    config.writeEntry("Num Entries",i);
    config.setGroup(key+" Lines");
    for (QMap<QString,unsigned>::ConstIterator it=completionInfo.lineNumbers.begin();
         it!=completionInfo.lineNumbers.end(); ++it)
      config.writeEntry(it.key(),*it);
  }
  config.sync();
}

TemplatePopup::TemplatePopup(Kate::View *parent)
  : QPopupMenu(parent), view(parent)
{
  connect(this,SIGNAL(activated(int)),this,SLOT(QPopupMenu_activated(int)));
  unsigned i=0;
  for (QValueList<QPair<QString,QString> >::ConstIterator it=preferences.templates.begin();
       it!=preferences.templates.end(); ++it, i++)
    insertItem((*it).first,i);
  QPoint pos=parent->cursorCoordinates();
  if (pos.x()<0 || pos.y()<0) {
    // Cursor outside of the view, so center on view instead.
    QSize parentSize=parent->size();
    QSize popupSize=sizeHint();
    pos.setX((parentSize.width()-popupSize.width())>>1);
    pos.setY((parentSize.height()-popupSize.height())>>1);
  }
  exec(parent->mapToGlobal(pos));
  deleteLater();
}

void TemplatePopup::QPopupMenu_activated(int id)
{
  QString code=preferences.templates[id].second;
  QString indent=view->currentTextLine();
  // Remove everything starting from the first non-whitespace character.
  indent=indent.remove(QRegExp("(?!\\s).*$"));
  indent.prepend('\n');
  code.replace('\n',indent);
  int cursorPos=code.find('|');
  if (cursorPos>=0) {
    QString left=code.left(cursorPos);
    QString right=code.mid(cursorPos+1);
    unsigned row, col;
    KTextEditor::EditInterfaceExt *editExt=KTextEditor::editInterfaceExt(view->getDoc());
    editExt->editBegin();
    view->insertText(left);
    view->cursorPositionReal(&row,&col);
    view->insertText(right);
    editExt->editEnd();
    view->setCursorPositionReal(row,col);
  } else view->insertText(code);
}

CompletionPopup::CompletionPopup(Kate::View *parent, const QString &fileName,
                                 MainForm *mainForm, QObject *receiver)
  : QObject(parent)
{
  connect(this,SIGNAL(closed()),receiver,SLOT(completionPopup_closed()));
  QValueList<KTextEditor::CompletionEntry> entries;
  if (!completionEntriesForFile(parent->getDoc()->text(),fileName,mainForm,
                                entries)) {
    emit closed();
    deleteLater();
    return;
  }
  entries=sortCompletionEntries(entries);
  unsigned column=parent->cursorColumnReal();
  int offset=0;
  if (column) {
    QString textLine=parent->currentTextLine();
    if (column<=textLine.length()) {
      while (column && (textLine[--column].isLetterOrNumber()
                        || textLine[column]=='_' || textLine[column]=='$'))
        offset++;
    }
  }
  connect(parent,SIGNAL(completionAborted()),this,SLOT(deleteLater()));
  connect(parent,SIGNAL(completionAborted()),this,SIGNAL(closed()));
  connect(parent,SIGNAL(completionDone()),this,SLOT(deleteLater()));
  connect(parent,SIGNAL(completionDone()),this,SIGNAL(closed()));
  parent->showCompletionBox(entries,offset);
}
