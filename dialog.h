#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

//============================================================================================================================================
class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_selectBaseDir_clicked();

    void on_pushButton_Generate_clicked();

private:
    Ui::Dialog *ui;

    QImage frame;
    QByteArray FramesData;
    QFile datafile;

    QStringList DefListDirs = {"01", "12", "20", "23", "30", "34", "45", "50", "56", "67", "78", "89", "90", "dotLeft", "dotRight"};
    QStringList GenListDirs = {"01", "12", "23", "34", "45", "56", "67", "78", "89", "90", "20", "30", "50", "dotLeft", "dotRight"};
    QStringList ImageFormats = {"*.BMP", "*.GIF", "*.JPG", "*.JPEG", "*.PNG", "*.PBM", "*.PGM", "*.PPM", "*.XBM", "*.XPM"};

    void SaveFrame();

protected:
    virtual void paintEvent(QPaintEvent *event);
};
//============================================================================================================================================

#endif // DIALOG_H
