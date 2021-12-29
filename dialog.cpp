#include <QFileDialog>
#include <QTime>
#include <QPainter>
#include <QThread>

#include "dialog.h"

#include "ui_dialog.h"

//============================================================================================================================================
Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);

    frame = QImage(135, 240, QImage::Format_RGB32);
    frame.fill(Qt::black);
}

//============================================================================================================================================
Dialog::~Dialog()
{
    delete ui;
}

//============================================================================================================================================
void Dialog::paintEvent(QPaintEvent *event)
{
     ui->label_1->setPixmap(QPixmap::fromImage(frame,Qt::NoFormatConversion));
}

//============================================================================================================================================
void Dialog::on_pushButton_selectBaseDir_clicked()
{
    QString BaseDirName = QFileDialog::getExistingDirectory(0, "Выбрать базовый каталог с изображениями", "");

    if ( !BaseDirName.isEmpty())
    {
        QDir katalog(BaseDirName);

        ui->pushButton_Generate->setEnabled(false);
        ui->lineEdit_selectBaseDir->clear();

        QStringList ListDirs = katalog.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name);

        if (ListDirs.size() != 15)
        {
            ui->textEdit_Log->append(QTime::currentTime().toString()+" Выбраный каталог неверной структуры");
            ui->textEdit_Log->append(QTime::currentTime().toString()+" неверное число подкаталогов");
            return;
        }
        if (ListDirs != DefListDirs)
        {
            ui->textEdit_Log->append(QTime::currentTime().toString()+" Выбраный каталог неверной структуры");
            foreach  (QString  subdir,  DefListDirs)
            {
                if (!ListDirs.contains(subdir, Qt::CaseInsensitive))
                {
                    ui->textEdit_Log->append(QTime::currentTime().toString() + " Отсутствует подкаталог " + subdir);
                }
            }
            return;
        }

        ui->textEdit_Log->append(QTime::currentTime().toString()+" Выбран базовый каталог с изображениями: " + BaseDirName);
        ui->lineEdit_selectBaseDir->setText(BaseDirName);

        ui->pushButton_Generate->setEnabled(true);
    }
    else
    {
        ui->textEdit_Log->append(QTime::currentTime().toString()+" Базовый каталог с изображениями не выбран");
        ui->pushButton_Generate->setEnabled(false);
        ui->lineEdit_selectBaseDir->clear();
    }

}

//============================================================================================================================================
void Dialog::on_pushButton_Generate_clicked()
{
    QDir katalog(ui->lineEdit_selectBaseDir->text());

    datafile.setFileName(katalog.path() + "/" + "data.bin");

    ui->pushButton_Generate->setEnabled(false);

    if (!datafile.open(QIODevice::WriteOnly))
    {
        ui->textEdit_Log->append(QTime::currentTime().toString()+" Невозможно открыть файл " + datafile.fileName());
        return;
    }

    FramesData.clear();

    foreach  (QString  subdir,  GenListDirs)
    {
        katalog.cd(subdir);

        QStringList ListFiles = katalog.entryList(ImageFormats, QDir::Files, QDir::IgnoreCase | QDir::Name);

        if (!((ListFiles.size() == 30) || (ListFiles.size() == 60)))
        {
            ui->textEdit_Log->append(QTime::currentTime().toString()+" неверное число файлов в подкаталоге " + katalog.absolutePath());
            return;
        }

        foreach  (QString  file,  ListFiles)
        {
            ui->label_1->update();
            QApplication::processEvents();
            QThread::msleep(10);

            ui->textEdit_Log->append(QTime::currentTime().toString()+" Загрузка файла " + katalog.absoluteFilePath(file));
            frame.load(katalog.absoluteFilePath(file));
            SaveFrame();
        }

        katalog.cdUp();
    }

    datafile.write(FramesData);
    datafile.close();
}

//============================================================================================================================================
void  Dialog::SaveFrame()
{

    for(int j = 0; j < 240; j++)
    {
        for (int i = 0; i < 135; i++)
        {
            QRgb pix = frame.pixel(i, j);

            uchar r5 = (uchar)( (float) qRed(pix)   * 31.0f / 255.0f + 0.5f );
            uchar g6 = (uchar)( (float) qGreen(pix) * 63.0f / 255.0f + 0.5f );
            uchar b5 = (uchar)( (float) qBlue(pix)  * 31.0f / 255.0f + 0.5f );

            FramesData.append( ((r5 & 0x1F) << 3) | ((g6 & 0x38) >> 3));
            FramesData.append( ((g6 & 0x07) << 5) | ((b5 & 0x1F) >> 0));
        }
    }
}

//============================================================================================================================================
