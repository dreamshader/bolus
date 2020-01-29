#include "dlgtimeblocks.h"
#include "ui_dlgtimeblocks.h"
#include "dlgtimeblockedit.h"

dlgTimeBlocks::dlgTimeBlocks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgTimeBlocks)
{
    int loopCount;
    int retVal;
    FILE *sysFP;
    char cmdBuff[40];
    char dataBuff[80];
    char newItem[40];

    pDlgTimeBlockEdit = nullptr;

    pParent = parent;

    ui->setupUi(this);

//    ui->lstTimeBlocks->addItem("00:00 - 06:00");
//    ui->lstTimeBlocks->addItem("06:00 - 12:00");
//    ui->lstTimeBlocks->addItem("12:00 - 16:00");
//    ui->lstTimeBlocks->addItem("16:00 - 22:00");
//    ui->lstTimeBlocks->addItem("22:00 - 00:00");

    retVal = system("~/bolus/bolus-cli -q -T#");

    if( retVal > 128 )
    {
        retVal = retVal >> 8;
    }

    if( retVal > 128 )
    {
        this->close();
    }


    for( loopCount = 0; loopCount < retVal; loopCount++ )
    {
        sprintf(cmdBuff, "~/bolus/bolus-cli -q -T%d", loopCount);
        if((sysFP = popen(cmdBuff, "r")) != nullptr )
        {
            fgets(dataBuff, 79, sysFP);
            fclose(sysFP);
            sprintf(newItem, "%c%c%c%c%c - %c%c%c%c%c",
                    dataBuff[0],dataBuff[1],dataBuff[2],dataBuff[3],dataBuff[4],
                    dataBuff[6],dataBuff[7],dataBuff[8],dataBuff[9],dataBuff[10] );
            ui->lstTimeBlocks->addItem(newItem);
        }

    }


//    printf("Std: %d, Min: %d\n", tmVal / 60, tmVal % 60);

}

dlgTimeBlocks::~dlgTimeBlocks()
{
    delete ui;
}

void dlgTimeBlocks::on_lstTimeBlocks_itemDoubleClicked(QListWidgetItem *item)
{
    int x = -1;

    if( item->text().compare("00:00 - 06:00") == 0 )
    {
        x = 0;
    }

    if( item->text().compare("06:00 - 11:00") == 0 )
    {
        x = 1;
    }

    if( item->text().compare("11:00 - 16:00") == 0 )
    {
        x = 2;
    }

    if( item->text().compare("16:00 - 22:00") == 0 )
    {
        x = 3;
    }

    if( item->text().compare("22:00 - 00:00") == 0 )
    {
        x = 4;
    }

    if( pDlgTimeBlockEdit == nullptr )
    {
        extern int currSelTimeBlock;
        currSelTimeBlock = x;

        pDlgTimeBlockEdit = new dlgTimeBlockEdit(pParent);
        pDlgTimeBlockEdit->show();
        pDlgTimeBlockEdit->update();

//        if( pDlgTimeBlockEdit->isActiveWindow() )
//        {
//            pDlgTimeBlockEdit->setTimeBlockIndex(x);
//            pDlgTimeBlockEdit->update();
//        }

        if( pDlgTimeBlockEdit->exec() == QDialog::Accepted )
        {

        }

        pDlgTimeBlockEdit = nullptr;
    }
}
