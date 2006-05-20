/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include <qaccel.h>
#include <kmessagebox.h>
#include <kurl.h>
#include "tpr.h"

extern tprSettings settings;

#define UnwrapLabel(theQLabel) ((theQLabel)->setAlignment((theQLabel)->alignment()&~WordBreak))
//Qt automatically sets RichText labels to have word wrap, which makes labels with underlines in them look bad.  This simply undoes the wrapping.

//manually created accelerators
QAccel *AOncalcVariableName_1;
QAccel *AOncalcVariableName_2;
QAccel *AGCCSwitches;
QAccel *AAsSwitches;
QAccel *AA68kSwitches;
QAccel *ACallAfterBuilding;
QAccel *AParameters;

void ProjectOptions::init()
{
  Boolean isregular;
  //Qt automatically sets QLabels to wrapped when they are RichText.  This undoes this change.
  UnwrapLabel(LOncalcVariableName_1);
  UnwrapLabel(LOncalcVariableName_2);
  UnwrapLabel(LGCCSwitches);
  UnwrapLabel(LAsSwitches);
  UnwrapLabel(LA68kSwitches);
  UnwrapLabel(LCallAfterBuilding);
  UnwrapLabel(LParameters);
  //Toggle controls to match settings.
  //Tab: General
  OncalcVariableName_1->setText(settings.data_var);
  if (settings.copy_data_var)
  {
    if (settings.copy_data_var_arc)
      CreateCopyIfArchived->toggle();
    else
      CreateCopyAlways->toggle();
  }
  OncalcVariableName_2->setText(settings.pack_name);
  isregular=TRUE;
  if (settings.fargo)
    FargoProgram->toggle(),isregular=FALSE;
  if (settings.flash_os)
    FlashOperatingSystem->toggle(),isregular=FALSE;
  if (settings.archive)
    FunctionArchive->toggle(),isregular=FALSE;
  if (isregular)
  {
    if (settings.use_data_var)
    {
      ExternalDataVariable->toggle();
      LOncalcVariableName_1->setEnabled(TRUE);
      OncalcVariableName_1->setEnabled(TRUE);
      LCreateCopyNever->setEnabled(TRUE);
      CreateCopyNever->setEnabled(TRUE);
      CreateCopyIfArchived->setEnabled(TRUE);
      CreateCopyAlways->setEnabled(TRUE);
    }
    if (settings.pack)
    {
      CompressProgram->toggle();
      LOncalcVariableName_2->setEnabled(TRUE);
      OncalcVariableName_2->setEnabled(TRUE);
    }
  }
  else
  {
    ExternalDataVariable->setEnabled(FALSE);
    CompressProgram->setEnabled(FALSE);
  }
  
  //Update stuff
  CheckOncalcNames();
  UpdateVisibilities();
  //Create accelerators for text boxes manually
  QAccel *accel; //for temporarily holding the accelerator pointer
  #define MakeAccelerator(destaccelptr,thewidget,thepage,thekey) destaccelptr=accel=new QAccel(PO_TabWidget->page(thepage)); \
    accel->connectItem(accel->insertItem(thekey),thewidget,SLOT(setFocus()));
  MakeAccelerator(AOncalcVariableName_1,OncalcVariableName_1,0,ALT+Key_V);
  MakeAccelerator(AOncalcVariableName_2,OncalcVariableName_2,0,ALT+Key_I);
  MakeAccelerator(AGCCSwitches,GCCSwitches,1,ALT+Key_G);
  MakeAccelerator(AAsSwitches,AsSwitches,1,ALT+Key_S);
  MakeAccelerator(AA68kSwitches,A68kSwitches,1,ALT+Key_A);
  MakeAccelerator(ACallAfterBuilding,CallAfterBuilding,3,ALT+Key_A);
  MakeAccelerator(AParameters,Parameters,3,ALT+Key_R);
  #undef MakeAccelerator
}

void ProjectOptions::destroy()
{
  delete(AOncalcVariableName_1);
  delete(AOncalcVariableName_2);
  delete(AGCCSwitches);
  delete(AAsSwitches);
  delete(AA68kSwitches);
  delete(ACallAfterBuilding);
  delete(AParameters);
  if (result()!=QDialog::Accepted)
    return;
  //Save settings
  //Tab: General
  settings.fargo=FALSE;
  settings.flash_os=FALSE;
  settings.archive=FALSE;
  settings.use_data_var=FALSE;
  settings.pack=FALSE;
  if (FargoProgram->isOn())
    settings.fargo=TRUE;
  else if (FlashOperatingSystem->isOn())
    settings.flash_os=TRUE;
  else if (FunctionArchive->isOn())
    settings.archive=TRUE;
  else if (RegularProgram->isOn()) //the original TIGCC IDE automatically unchecked External data variable and Compress program when you toggled away from Regular Program, so we will only consider these two if Regular Program is toggled
  {
    if (ExternalDataVariable->isOn())
      settings.use_data_var=TRUE;
    if (CompressProgram->isOn())
      settings.pack=TRUE;
  }
  //Subselections for External data variable and Compress program are always preserved.
  settings.data_var=OncalcVariableName_1->text();
  settings.copy_data_var_arc=FALSE;
  if (CreateCopyNever->isOn())
    settings.copy_data_var=FALSE;
  else
  {
    settings.copy_data_var=TRUE;
    if (CreateCopyIfArchived->isOn())
      settings.copy_data_var_arc=TRUE;
  }
  settings.pack_name=OncalcVariableName_2->text();
}

void ProjectOptions::RegularProgramToggle()
{
  int state;
  buttonOk->setAccel(0);
  buttonOk->setText("OK");
  state=RegularProgram->isOn()?TRUE:FALSE;
  if (!state)
  {
    if (ExternalDataVariable->isOn())
      ExternalDataVariable->toggle();
    if (CompressProgram->isOn())
      CompressProgram->toggle();
  }
  ExternalDataVariable->setEnabled(state);
  CompressProgram->setEnabled(state);
  ExternalDataVariableToggle();
  CompressProgramToggle();
}


void ProjectOptions::ExternalDataVariableToggle()
{
  int state;
  state=ExternalDataVariable->isOn()?TRUE:FALSE;
  LOncalcVariableName_1->setEnabled(state);
  OncalcVariableName_1->setEnabled(state);
  LCreateCopyNever->setEnabled(state);
  CreateCopyNever->setEnabled(state);
  CreateCopyIfArchived->setEnabled(state);
  CreateCopyAlways->setEnabled(state);
  CheckOncalcNames();
}


void ProjectOptions::CompressProgramToggle()
{
  int state;
  state=CompressProgram->isOn()?TRUE:FALSE;
  LOncalcVariableName_2->setEnabled(state);
  OncalcVariableName_2->setEnabled(state);
  CheckOncalcNames();
}


void ProjectOptions::CheckOncalcNames()
{
  const QString &edvname=OncalcVariableName_1->text();
  const QString &compname=OncalcVariableName_2->text();
  Boolean edvon=OncalcVariableName_1->isEnabled();
  Boolean compon=OncalcVariableName_2->isEnabled();
  if ((edvon&&edvname.isEmpty()) || (compon&&compname.isEmpty()) || (edvon&&compon&&!edvname.compare(compname)))
    buttonOk->setEnabled(FALSE);
  else
    buttonOk->setEnabled(TRUE);
}


void ProjectOptions::UpdateVisibilities()
{
  Boolean regularprogram=RegularProgram->isOn();
  Boolean functionarchive=FunctionArchive->isOn();
  if (regularprogram)
    ProgramOptions->show();
  else
    ProgramOptions->hide();
  PO_TabWidget->setTabEnabled(PO_TabWidget->page(2),!functionarchive);
  PO_TabWidget->setTabEnabled(PO_TabWidget->page(3),!functionarchive);
  
}

void ProjectOptions::ProgramOptionsFunc()
{
  
}
