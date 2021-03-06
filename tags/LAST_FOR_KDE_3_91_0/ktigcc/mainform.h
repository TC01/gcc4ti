/*
   ktigcc - TIGCC IDE for KDE

   Copyright (C) 2007 Kevin Kofler

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

#pragma once

#include "ui_mainform.h"

#include "tiemu.h"

namespace KTextEditor {
  class Document;
  class View;
  class Cursor;
}

class MainForm : public QMainWindow, public Ui::MainForm
{
    Q_OBJECT

public:
    MainForm(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~MainForm();

    static void deleteErrorsForLVFile( Q3ListViewItem * item );
    static void deleteErrorsForSrcFile( void * srcFile );
    static void createErrorCursorsForSourceFile( Q3ListViewItem * item );
    static void deleteOverwrittenErrorsIn( void * srcFile );
    static QString SGetFileName( int mode, const QString & fileFilter, const QString & caption, QWidget * parent );
    static QStringList SGetFileName_Multiple( const QString & fileFilter, const QString & caption, QWidget * parent );
    void * createView( const QString & fileName, const QString & fileText, Q3ListViewItem * category );
    virtual void adoptSourceFile( void * srcFile );
    virtual bool openProject( const QString & fileName );
    int fileSavePrompt( Q3ListViewItem * fileItem );
    virtual void fileSave_fromto( const QString & lastProj, const QString & nextProj );
    virtual void findAndOpenFile( const QString & fileName, void * category );
    virtual void openHeader( const QString & fileName, bool systemHeader, int lineno );
    virtual QString writeTempSourceFile( void * srcFile, bool inProject );
    virtual void compileSourceFile( void * srcFile );
    static QString tilibsErrorMessage( int err );
    virtual QString textForHeader( const QString & fileName );
    bool removeItem( Q3ListViewItem * item );

public slots:
    virtual void errorListView_clicked( Q3ListViewItem * item );
    virtual bool findSourceFile( bool & inProject, void * & srcFile, const QString & fileName );
    virtual void te_popup_aboutToShow();
    virtual void te_popup_activated( int index );
    virtual void shortcut_0_activated();
    virtual void shortcut_1_activated();
    virtual void shortcut_2_activated();
    virtual void shortcut_3_activated();
    virtual void shortcut_4_activated();
    virtual void shortcut_5_activated();
    virtual void shortcut_6_activated();
    virtual void shortcut_7_activated();
    virtual void shortcut_8_activated();
    virtual void shortcut_9_activated();
    virtual void shortcut_10_activated();
    virtual void shortcut_11_activated();
    virtual void shortcut_12_activated();
    virtual void shortcut_13_activated();
    virtual void shortcut_14_activated();
    virtual void shortcut_15_activated();
    virtual void shortcut_16_activated();
    virtual void completionPopup_closed();
    virtual void fileNewProject();
    virtual void fileOpen();
    virtual void fileRecent1();
    virtual void fileRecent2();
    virtual void fileRecent3();
    virtual void fileRecent4();
    virtual bool fileSave();
    virtual bool fileSaveAs();
    virtual void filePrint();
    virtual void filePrintQuickly();
    virtual void filePreferences();
    virtual void editUndo();
    virtual void editRedo();
    virtual void editClear();
    virtual void editCut();
    virtual void editCopy();
    virtual void editPaste();
    virtual void editSelectAll();
    virtual void editIncreaseIndent();
    virtual void editDecreaseIndent();
    virtual void findFind();
    virtual void findFind_next();
    virtual void findFind_highlight( const QString & text, int matchingindex, int matchedlength );
    virtual void findFind_stop();
    virtual void findReplace();
    virtual void findReplace_next();
    virtual void findReplace_highlight( const QString & text, int matchingindex, int matchedlength );
    virtual void findReplace_replace( const QString & text, int replacementIndex, int replacedLength, int matchedLength );
    virtual void findReplace_stop();
    virtual void findFunctions();
    virtual void findFunctions_functionListBox_highlighted( int index );
    virtual void findFunctions_functionListBox_selected( int index );
    virtual void findFunctions_prototypeButton_clicked();
    virtual void findFunctions_implementationButton_clicked();
    virtual void findFunctionsPopup_aboutToShow();
    virtual void findFunctionsPopup_aboutToHide();
    virtual void findFunctionsPopup_aboutToHide_async();
    virtual void findFunctionsPopup_activated( int id );
    virtual void findOpenFileAtCursor();
    virtual void findFindSymbolDeclaration();
    virtual void projectAddFiles();
    virtual void process_finished();
    virtual void process_readyRead();
    virtual void process_readyRead_recordOnly();
    virtual void projectCompile();
    virtual void projectMake();
    virtual void projectBuild();
    virtual void projectStopCompilation();
    virtual void projectForceQuit();
    virtual void projectErrorsAndWarnings( bool on );
    virtual void projectProgramOutput();
    virtual void projectOptions();
    virtual void debugRun();
    virtual void debugPause();
    virtual void debugReset();
    virtual void toolsConfigure();
    virtual void toolsMenu_highlighted( int id );
    virtual void toolsMenu_activated( int id );
    virtual void toolsMenu_aboutToHide();
    virtual void helpDocumentation();
    virtual void helpContents();
    virtual void helpIndex();
    virtual void helpSearch();
    virtual void helpNews();
    virtual void helpAbout();
    virtual void resizeEvent( QResizeEvent * event );
    virtual void timerEvent( QTimerEvent * event );
    virtual void fileTreeClicked( Q3ListViewItem * item );
    virtual void fileNewFolder();
    virtual void fileTreeContextMenuRequested( Q3ListViewItem * item, const QPoint & pos, int col );
    virtual void fileNewCHeader();
    virtual void fileNewGNUAssemblyHeader();
    virtual void fileNewA68kAssemblyHeader();
    virtual void fileNewCSourceFile();
    virtual void fileNewGNUAssemblySourceFile();
    virtual void fileNewA68kAssemblySourceFile();
    virtual void fileNewQuillSourceFile();
    virtual void fileNewTextFile();
    virtual void statusBar_messageChanged(const QString &message);
    virtual void current_view_cursorPositionChanged(KTextEditor::View *view, const KTextEditor::Cursor &newPosition);
    virtual void current_view_textChanged(KTextEditor::Document *document);
    virtual void current_view_undoChanged();
    virtual void current_view_selectionChanged(KTextEditor::View *view);
    virtual void current_view_textInserted(KTextEditor::View *view, const KTextEditor::Cursor &position, const QString &text);
    virtual void clipboard_dataChanged();
    virtual void fileTreeItemRenamed( Q3ListViewItem * item, const QString & newName, int col );
    virtual void KDirWatch_dirty( const QString & fileName );

protected:
    virtual void closeEvent( QCloseEvent * e );

protected slots:
    virtual void languageChange();

private:
    void clearProject();
    QString findFilter( unsigned short job );
    void updateRecent();
    void addRecent( const QString & fileName );
    Q3ListViewItem * openFile( Q3ListViewItem * category, Q3ListViewItem * parent, const QString & fileCaption, const QString & fileName );
    static Q3ListViewItem * createFolder( Q3ListViewItem * parent, const QString & name );
    void fileOpen_addList( Q3ListViewItem * category, void * fileListV, void * dir, const QString & open_file );
    int savePrompt( void );
    void removeTrailingSpacesFromView( void * view );
    void fileSave_save( Q3ListViewItem * theItem );
    void fileSave_saveAs( Q3ListViewItem * theItem );
    void fileSave_loadList( Q3ListViewItem * category, void * fileListV, const QString & base_dir, void * dir_new, QString * open_file );
    void findReplace_next( bool firstTime );
    int projectAddFiles_oneFile( const QString & fileName );
    void startCompiling();
    void stopCompiling();
    void compileFile( void * srcFile, bool inProject, bool force );
    void compileProject( bool forceAll );
    void linkProject();
    void showStats();
    bool tiemuInstance(TiEmuDBus *&instance);
    void updateToolsMenu();
    void updateSizes();
    QStringList extractAllFileNames( void );
    void extractFileTreeInfo( Q3ListViewItem * parent, Q3ListViewItem * * p_category, QString * p_folderPath );
    void newFile( Q3ListViewItem * parent, QString text, const QPixmap & pixmap );
    void newFile( Q3ListViewItem * parent );
    Q3ListViewItem * resolveParent( Q3ListViewItem * category );
    void updateLeftStatusLabel();
    void updateRightStatusLabel();
    void current_view_newLineHook();
    QString pathInProject( Q3ListViewItem * item );
    void shortcutActivated(int index);

};

namespace KTextEditor {
  class View;
};
void sendCommand(KTextEditor::View *view, const QString &cmd);
void setTabWidth(KTextEditor::View *view, unsigned tabWidth);
