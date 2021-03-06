/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/

/*
   ktigcc - TIGCC IDE for KDE

   Copyright (C) 2004 Kevin Kofler

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
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <qlabel.h>
#include <qstatusbar.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <qdragobject.h>
#include <cstdio>
using std::puts;

extern const char *tigcc_base;
extern const char *quill_drv;

// All the methods are inline because otherwise QT Designer will mistake them
// for slots of the main form.
class ListViewFolder : public QListViewItem {
  public:
  ListViewFolder(QListView *parent) : QListViewItem(parent)
  {
    setPixmap(0,QPixmap::fromMimeSource("folder1.png"));    
    setDragEnabled(TRUE);
    setDropEnabled(TRUE);
  }
  ListViewFolder(QListViewItem *parent) : QListViewItem(parent)
  {
    setPixmap(0,QPixmap::fromMimeSource("folder1.png"));
    setDragEnabled(TRUE);
    setDropEnabled(TRUE);
  }
  ListViewFolder(QListView *parent, QListViewItem *after)
          : QListViewItem(parent, after)
  {
    setPixmap(0,QPixmap::fromMimeSource("folder1.png"));
    setDropEnabled(TRUE);
    setDragEnabled(TRUE);
  }
  ListViewFolder(QListViewItem *parent, QListViewItem *after)
          : QListViewItem(parent, after)
  {
    setPixmap(0,QPixmap::fromMimeSource("folder1.png"));
    setDragEnabled(TRUE);
    setDropEnabled(TRUE);
  }
  virtual int rtti(void) const {return 0x716CC0;}
  virtual bool acceptDrop(const QMimeSource *mime) const {
    puts("folder acceptDrop called");
    return (mime->provides("x-ktigcc-folder")
            ||mime->provides("x-ktigcc-file"));
  }
  protected:
  virtual void dropped (QDropEvent *e) {
    puts("folder dropped called");
  }
  virtual void dragEnterEvent ( QDragEnterEvent * ) {
    puts("folder dragEnterEvent called");
  }
};

class ListViewFile : public QListViewItem {
  public:
  ListViewFile(QListView *parent) : QListViewItem(parent)
  {
    setPixmap(0,QPixmap::fromMimeSource("filex.png"));    
    setDragEnabled(TRUE);
    setDropEnabled(TRUE);
    setRenameEnabled(0,TRUE);
  }
  ListViewFile(QListViewItem *parent) : QListViewItem(parent)
  {
    setPixmap(0,QPixmap::fromMimeSource("filex.png"));
    setDragEnabled(TRUE);
    setDropEnabled(TRUE);
    setRenameEnabled(0,TRUE);
  }
  ListViewFile(QListView *parent, QListViewItem *after)
          : QListViewItem(parent, after)
  {
    setPixmap(0,QPixmap::fromMimeSource("filex.png"));
    setDropEnabled(TRUE);
    setDragEnabled(TRUE);
    setRenameEnabled(0,TRUE);
  }
  ListViewFile(QListViewItem *parent, QListViewItem *after)
          : QListViewItem(parent, after)
  {
    setPixmap(0,QPixmap::fromMimeSource("filex.png"));
    setDragEnabled(TRUE);
    setDropEnabled(TRUE);
    setRenameEnabled(0,TRUE);
  }
  virtual int rtti(void) const {return 0x716CC1;}
  bool acceptDrop(const QMimeSource *mime) const {
    puts("file acceptDrop called");
    return (mime->provides("x-ktigcc-folder")
            ||mime->provides("x-ktigcc-file"));
  }
  QString textBuffer;
  protected:
  virtual void dropped (QDropEvent *e) {
    puts("file dropped called");
  }
  virtual void dragEnterEvent ( QDragEnterEvent * ) {
    puts("file dragEnterEvent called");
  }
};

// These should be instance variables in clean C++, but QT Designer won't let me
// touch the class definition, so this is all I can do. And there is only one
// instance of MainForm anyway.
static QListViewItem *rootListItem;
static QListViewItem *hFilesListItem;
static QListViewItem *cFilesListItem;
static QListViewItem *sFilesListItem;
static QListViewItem *asmFilesListItem;
static QListViewItem *qllFilesListItem;
static QListViewItem *oFilesListItem;
static QListViewItem *aFilesListItem;
static QListViewItem *txtFilesListItem;
static QListViewItem *othFilesListItem;
static QListViewItem *currentListItem;
static QLabel *leftStatusLabel;
static QLabel *rightStatusLabel;
static int fileCount=0, hFileCount=0, cFileCount=0, sFileCount=0, asmFileCount=0, qllFileCount=0, oFileCount=0, aFileCount=0, txtFileCount=0, othFileCount=0;

class DnDListView : public QListView {
  private:
  public:
  DnDListView ( QWidget * parent = 0, const char * name = 0, WFlags f = 0 )
          : QListView(parent,name,f) {}
  bool acceptDrop(const QMimeSource *mime) const {
    puts("acceptDrop called");
    return (mime->provides("x-ktigcc-folder")
            ||mime->provides("x-ktigcc-file"));
  }
  protected:
  virtual QDragObject *dragObject() {
    puts("dragObject called");
    QStoredDrag *storedDrag=new QStoredDrag("x-ktigcc-folder");
    QByteArray data(sizeof(QListViewItem*));
    data.duplicate(reinterpret_cast<char *>(&currentListItem),
                   sizeof(QListViewItem*));
    storedDrag->setEncodedData(data);
    storedDrag->drag();
    puts("dragObject returning");
    return storedDrag;
  }
  virtual void dropped (QDropEvent *e) {
    puts("dropped called");
  }
  virtual void dragEnterEvent ( QDragEnterEvent * ) {
    puts("dragEnterEvent called");
  }
};

void MainForm::init()
{  
  fileNewFolderAction->setEnabled(FALSE);
  QValueList<int> list;
  list.append(150);
  list.append(500);
  splitter->setSizes(list);
  leftStatusLabel=new QLabel("0 Files Total",this);
  leftStatusLabel->setMaximumWidth(splitter->sizes().first());
  statusBar()->addWidget(leftStatusLabel,1);
  rightStatusLabel=new QLabel("",this);
  rightStatusLabel->setMaximumWidth(splitter->sizes().last());
  statusBar()->addWidget(rightStatusLabel,1);
  statusBar()->setSizeGripEnabled(FALSE);
  fileTree->setSorting(-1);
  fileTree->setColumnWidthMode(0,QListView::Maximum);
  rootListItem=new QListViewItem(fileTree);
  rootListItem->setText(0,"Project1");
  rootListItem->setPixmap(0,QPixmap::fromMimeSource("tpr.png"));
  rootListItem->setOpen(TRUE);
  rootListItem->setDragEnabled(TRUE);
  QListViewItem *folderListItem=new ListViewFolder(rootListItem);
  hFilesListItem=folderListItem;
  folderListItem->setText(0,"Header Files");
  folderListItem=new ListViewFolder(rootListItem,folderListItem);
  cFilesListItem=folderListItem;
  folderListItem->setText(0,"C Files");
  folderListItem=new ListViewFolder(rootListItem,folderListItem);
  sFilesListItem=folderListItem;
  folderListItem->setText(0,"GNU Assembly Files");
  folderListItem=new ListViewFolder(rootListItem,folderListItem);
  asmFilesListItem=folderListItem;
  folderListItem->setText(0,"A68k Assembly Files");
  if (quill_drv) {
    folderListItem=new ListViewFolder(rootListItem,folderListItem);
    qllFilesListItem=folderListItem;
    folderListItem->setText(0,"Quill Files");
  } else {
    qllFilesListItem=NULL;
    fileNewQuillSourceFileAction->setVisible(FALSE);
  }
  folderListItem=new ListViewFolder(rootListItem,folderListItem);
  oFilesListItem=folderListItem;
  folderListItem->setText(0,"Object Files");
  folderListItem=new ListViewFolder(rootListItem,folderListItem);
  aFilesListItem=folderListItem;
  folderListItem->setText(0,"Archive Files");
  folderListItem=new ListViewFolder(rootListItem,folderListItem);
  txtFilesListItem=folderListItem;
  folderListItem->setText(0,"Text Files");
  folderListItem=new ListViewFolder(rootListItem,folderListItem);
  othFilesListItem=folderListItem;
  folderListItem->setText(0,"Other Files");
  startTimer(100);
}

void MainForm::fileNewProject()
{
  rootListItem->setText(0,"Project1");
  fileTreeClicked(rootListItem);
  QListViewItem *f, *next;
  for (f=hFilesListItem->firstChild();f;f=next) {
    next=f->nextSibling();
    delete f;
  }
  for (f=cFilesListItem->firstChild();f;f=next) {
    next=f->nextSibling();
    delete f;
  }
  for (f=sFilesListItem->firstChild();f;f=next) {
    next=f->nextSibling();
    delete f;
  }
  for (f=asmFilesListItem->firstChild();f;f=next) {
    next=f->nextSibling();
    delete f;
  }
  if (qllFilesListItem) {
    for (f=qllFilesListItem->firstChild();f;f=next) {
      next=f->nextSibling();
      delete f;
    }
  }
  for (f=oFilesListItem->firstChild();f;f=next) {
    next=f->nextSibling();
    delete f;
  }
  for (f=aFilesListItem->firstChild();f;f=next) {
    next=f->nextSibling();
    delete f;
  }
  for (f=txtFilesListItem->firstChild();f;f=next) {
    next=f->nextSibling();
    delete f;
  }
  for (f=othFilesListItem->firstChild();f;f=next) {
    next=f->nextSibling();
    delete f;
  }
  fileCount=cFileCount=hFileCount=sFileCount=asmFileCount=qllFileCount=oFileCount=aFileCount=txtFileCount=othFileCount=0;
  updateLeftStatusLabel();
}

void MainForm::fileOpen()
{
  
}

void MainForm::fileSave()
{
  
}

void MainForm::fileSaveAs()
{
  
}

void MainForm::filePrint()
{
  
}

void MainForm::fileExit()
{
  
}

void MainForm::editUndo()
{
  
}

void MainForm::editRedo()
{
  
}

void MainForm::editCut()
{
  
}

void MainForm::editCopy()
{
  
}

void MainForm::editPaste()
{
  
}

void MainForm::editFind()
{
  
}

void MainForm::helpIndex()
{
  
}

void MainForm::helpContents()
{
  
}

void MainForm::helpAbout()
{
  
}

void MainForm::updateSizes()
{
  int leftSize=splitter->sizes().first();
  int rightSize=splitter->sizes().last();
  int totalSize=leftSize+rightSize;
  int mySize=size().width();
  leftStatusLabel->setMaximumWidth(leftSize*mySize/totalSize);
  rightStatusLabel->setMaximumWidth(rightSize*mySize/totalSize-10>0?
                                    rightSize*mySize/totalSize-10:0);
}

void MainForm::resizeEvent(QResizeEvent *event)
{
  QMainWindow::resizeEvent(event);
  if (event->size()==event->oldSize()) return;
  updateSizes();
}

void MainForm::timerEvent(QTimerEvent *event)
{
  static int lastSplitterPos=-1;
  QMainWindow::timerEvent(event);
  if (lastSplitterPos==splitter->sizes().first()) return;
  lastSplitterPos=splitter->sizes().first();
  updateSizes();
}


void MainForm::fileTreeClicked(QListViewItem *item)
{
  if (!item) return;
  if (currentListItem && currentListItem->rtti()==0x716CC0)
    currentListItem->setPixmap(0,QPixmap::fromMimeSource("folder1.png"));
  if (currentListItem && currentListItem->rtti()==0x716CC1)
    static_cast<ListViewFile *>(currentListItem)->textBuffer=kTextEdit1->text();
  if (item->rtti()==0x716CC0) {
    item->setPixmap(0,QPixmap::fromMimeSource("folder2.png"));
    fileNewFolderAction->setEnabled(TRUE);
    kTextEdit1->setEnabled(FALSE);
    kTextEdit1->setText("");
    kTextEdit1->setPaletteBackgroundColor(QColor(230,230,230));
  } else if (item->rtti()==0x716CC1) {
    fileNewFolderAction->setEnabled(TRUE);
    kTextEdit1->setEnabled(TRUE);
    kTextEdit1->setText(static_cast<ListViewFile *>(item)->textBuffer);
    kTextEdit1->setPaletteBackgroundColor(QColor(255,255,255));
  } else {
    fileNewFolderAction->setEnabled(FALSE);
    kTextEdit1->setText("");
    kTextEdit1->setPaletteBackgroundColor(QColor(230,230,230));
  }
  currentListItem=item;
  updateLeftStatusLabel();
}


void MainForm::fileNewFolder()
{
  if (currentListItem->rtti()==0x716CC1)
    currentListItem=currentListItem->parent();
  QListViewItem *item=NULL, *next=currentListItem->firstChild();
  for (; next && next->rtti()==0x716CC0; next=item->nextSibling())
    item=next;
  QListViewItem *newFolder=item?new ListViewFolder(currentListItem,item)
                           :new ListViewFolder(currentListItem);
  newFolder->setText(0,"NewFolder");
  newFolder->setRenameEnabled(0,TRUE);
  currentListItem->setOpen(TRUE);
  fileTreeClicked(newFolder);
  newFolder->startRename(0);
}

#define unused_col __attribute__((unused)) col /* stupid QT designer... */
void MainForm::fileTreeContextMenuRequested(QListViewItem *item,
                                            const QPoint &pos,
                                            int unused_col)
{
  fileTreeClicked(item);
  if (item && item->rtti()==0x716CC0) {
    QPopupMenu menu;
    menu.insertItem("New &Folder",0);
    menu.insertItem("New F&ile",1);
    QListViewItem *category=item;
    while (category->parent()->rtti()==0x716CC0) category=category->parent();
    if (category==oFilesListItem || category==aFilesListItem
        || category==othFilesListItem) menu.setItemEnabled(1,FALSE);
    if (!(item==hFilesListItem || item==cFilesListItem || item==sFilesListItem
          || item==asmFilesListItem || item && item==qllFilesListItem
          || item==oFilesListItem || item==aFilesListItem
          || item==txtFilesListItem || item==othFilesListItem)) {
      menu.insertSeparator();
      menu.insertItem("&Remove",2);
      menu.insertItem("Re&name",3);
    }
    switch (menu.exec(pos)) {
      case 0:
        fileNewFolder();
        break;
      case 1:
        newFile(item);
        break;
      case 2:
        delete item;
        currentListItem=NULL;
        fileTreeClicked(fileTree->currentItem());
        break;
      case 3:
        item->startRename(0);
    }
  }
}

void MainForm::newFile( QListViewItem *parent, QString text, const char *iconName )
{
  QListViewItem *item=NULL, *next=parent->firstChild();
  for (; next; next=item->nextSibling())
    item=next;
  ListViewFile *newFile=item?new ListViewFile(parent,item)
                        :new ListViewFile(parent);
  newFile->setText(0,"New File");
  newFile->setPixmap(0,QPixmap::fromMimeSource(iconName));
  parent->setOpen(TRUE);
  fileTreeClicked(newFile);
  newFile->startRename(0);
  kTextEdit1->setText(text);
  fileCount++;
  QListViewItem *category=parent;
  while (category->parent()->rtti()==0x716CC0) category=category->parent();
  (category==hFilesListItem?hFileCount:category==cFilesListItem?cFileCount:
   category==sFilesListItem?sFileCount:category==asmFilesListItem?asmFileCount:
   category==qllFilesListItem?qllFileCount:category==oFilesListItem?oFileCount:
   category==aFilesListItem?aFileCount:category==txtFilesListItem?txtFileCount:
   othFileCount)++;
  updateLeftStatusLabel();
}

void MainForm::newFile( QListViewItem *parent )
{
  QListViewItem *category=parent;
  while (category->parent()->rtti()==0x716CC0) category=category->parent();
  newFile(parent,category==txtFilesListItem?"":
                 ((category==hFilesListItem?"// Header File\n//":
                  category==cFilesListItem?"// C Source File\n//":
                  category==sFilesListItem?"| Assembly Source File\n|":
                  category==asmFilesListItem?"; Assembly Source File\n;":
                  category==qllFilesListItem?"// Quill Source File\n//":"???\n")
                 +QString(" Created ")
                 +QDateTime::currentDateTime ().toString(Qt::LocalDate)+"\n"+
                 QString(category==cFilesListItem?(cFileCount?
                 "\n#include <tigcclib.h>\n":
                 "\n// Delete or comment out the items you do not need.\n"
                 "#define COMMENT_STRING         \"Place your comment here.\"\n"
                 "#define COMMENT_PROGRAM_NAME   "
                 "\"Place your program name here.\"\n"
                 "#define COMMENT_VERSION_STRING "
                 "\"Place your version string here.\"\n"
                 "#define COMMENT_VERSION_NUMBER 0,0,0,0 "
                 "/* major, minor, revision, subrevision */\n"
                 "#define COMMENT_BW_ICON \\\n"
                 "\t{0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000}\n"
                 "#define COMMENT_GRAY_ICON \\\n"
                 "\t{0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000},\n"
                 "\t{0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000, \\\n"
                 "\t 0b0000000000000000}\n\n#include <tigcclib.h>\n\n"
                 "// Main Function\nvoid _main(void)\n{\n"
                 "\t// Place your code here.\n}\n"):"")),
                 category==cFilesListItem||category==qllFilesListItem
                                                    ?"filec.png":
                 category==hFilesListItem?"fileh.png":
                 category==sFilesListItem||category==asmFilesListItem
                                                    ?"files.png":
                 category==txtFilesListItem?"filet.png":"filex.png");
}


void MainForm::fileNewCHeader()
{
  newFile(hFilesListItem,"// Header File\n// Created "+QDateTime::currentDateTime ().toString(Qt::LocalDate)+"\n","fileh.png");
}


void MainForm::fileNewGNUAssemblyHeader()
{
  newFile(hFilesListItem,"| Header File\n| Created "+QDateTime::currentDateTime ().toString(Qt::LocalDate)+"\n","fileh.png");
}


void MainForm::fileNewA68kAssemblyHeader()
{
  newFile(hFilesListItem,"; Header File\n; Created "+QDateTime::currentDateTime ().toString(Qt::LocalDate)+"\n","fileh.png");
}


void MainForm::fileNewCSourceFile()
{
  newFile(cFilesListItem,"// C Source File\n// Created "+QDateTime::currentDateTime ().toString(Qt::LocalDate)+"\n","filec.png");
}


void MainForm::fileNewGNUAssemblySourceFile()
{
  newFile(sFilesListItem,"| Assembly Source File\n| Created "+QDateTime::currentDateTime ().toString(Qt::LocalDate)+"\n","files.png");
}


void MainForm::fileNewA68kAssemblySourceFile()
{
  newFile(asmFilesListItem,"; Assembly Source File\n; Created "+QDateTime::currentDateTime ().toString(Qt::LocalDate)+"\n","files.png");
}


void MainForm::fileNewQuillSourceFile()
{
  newFile(qllFilesListItem,"// Quill Source File\n// Created "+QDateTime::currentDateTime ().toString(Qt::LocalDate)+"\n","filec.png");
}


void MainForm::fileNewTextFile()
{
  newFile(txtFilesListItem,"","filet.png");
}


void MainForm::updateLeftStatusLabel()
{
  QString text=QString::number(fileCount)+QString(" File")
               +QString(fileCount!=1?"s":"")+QString(" Total");
  QListViewItem *category=currentListItem;
  if (currentListItem->rtti()==0x716CC0||currentListItem->rtti()==0x716CC1) {
    while (category->parent()->rtti()==0x716CC0) category=category->parent();
    text+=QString(", ")+QString::number(category==hFilesListItem?hFileCount:
                                        category==cFilesListItem?cFileCount:
                                        category==sFilesListItem?sFileCount:
                                        category==asmFilesListItem?asmFileCount:
                                        category==qllFilesListItem?qllFileCount:
                                        category==oFilesListItem?oFileCount:
                                        category==aFilesListItem?aFileCount:
                                        category==txtFilesListItem?txtFileCount:
                                        othFileCount)
          +QString(" in Category");
  }
  leftStatusLabel->setText(text);
}

// Yes, this is an ugly hack... Any better suggestions?
#define QListView DnDListView
