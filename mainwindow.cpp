#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <read_data.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->button_C,SIGNAL(pressed()),this,SLOT(button_pressed()));
    connect(ui->button_Cmax,SIGNAL(pressed()),this,SLOT(button_pressed()));
    connect(ui->button_PI,SIGNAL(pressed()),this,SLOT(button_pressed()));
    connect(ui->button_p,SIGNAL(pressed()),this,SLOT(button_pressed()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::button_pressed()
{
    QPushButton *button = (QPushButton*)sender();
    ui->Button_pressed_label->setText(button->text());
}

void MainWindow::change_LCD_number(int number)
{
    ui->LCD_Cmax->display(number);
}
void MainWindow::table_init(vector<int>& number)
{
    QTableWidgetItem *item = new QTableWidgetItem();
    ui->tableWidget->setRowCount(3);
    ui->tableWidget->horizontalHeader()->setVisible(false);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
  l_iteracji = arg1;
  std::cout << "ASD: " << arg1 << std::endl;
}

void MainWindow::fillTable()
// Tabela i wypelnienie danych
{
  int rowIncrementator = 0;

  for(auto &time : FFS.best_C)
  {
    std::cout << "Czas: " << time << std::endl;
    ui->tableWidget->insertRow(rowIncrementator);

    QTableWidgetItem* item = new QTableWidgetItem(QString::number(time));
    QTableWidgetItem* item2 = new QTableWidgetItem(QString::number(rowIncrementator));

    ui->tableWidget->setItem(rowIncrementator, 1, item);
    ui->tableWidget->setItem(rowIncrementator, 0, item2);

    rowIncrementator++;

  }
}

void MainWindow::fillTable2()
{
    int rowIncrementator = 0;
    int nr_stanowiska = 1;
    int l_maszyn = 1;

    for (auto &operation : FFS.best_PI)
    {
        if (operation == 0 && rowIncrementator > 0)
        {
            l_maszyn++;
            if (l_maszyn > FFS.getMs())
            {
                l_maszyn = 1;
                nr_stanowiska++;
            }
            continue;
        }
        if (rowIncrementator != 0 )
        {
            ui->tableWidget_2->insertRow(rowIncrementator - 1);
            QTableWidgetItem* item = new QTableWidgetItem(QString::number(nr_stanowiska));
            QTableWidgetItem* item2 = new QTableWidgetItem(QString::number(l_maszyn));
            QTableWidgetItem* item3 = new QTableWidgetItem(QString::number(operation));
            ui->tableWidget_2->setItem(rowIncrementator - 1, 0, item);
            ui->tableWidget_2->setItem(rowIncrementator - 1, 1, item2);
            ui->tableWidget_2->setItem(rowIncrementator - 1, 2, item3);
        }
        rowIncrementator++;
    }


}
void MainWindow::on_Licz_clicked()
// Oblicza bo wybraniu sciezki z plikiem txt i po wybraniu liczby iteracji
{
    FFS.find_best_perm(sciezka,l_iteracji);
    change_LCD_number(FFS.return_Cmax());
    fillTable();
    fillTable2();
}

void MainWindow::on_wczytaj_plik_clicked()
//Wczytywanie pliku zmienna wczytaj zmieniona na string nie przyjmuje charow i zamiania \\ na //
{
    QString wczytaj=QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "C:\\Pempera_projekt\\FlexFlowShop\\", "Text File (*txt)");
    QDir::toNativeSeparators(wczytaj);
    sciezka = wczytaj.toStdString();
    QMessageBox::information(this,tr("Wybrano Plik !"),wczytaj);


}
