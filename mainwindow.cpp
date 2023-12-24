#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <vector>
#include <input_file.h>
#include <algorithm>
#include <QListWidget>
#include <pugixml.hpp>
#include <compare_with_samples.h>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlock>
#include <QFontMetrics>
#include <QScrollBar>
using namespace std;
void remove_all_nl(std::vector<char>& arr);
vector<char> zero_file;
vector <int> result_arr;
vector<vector<char>> samples;
vector<char> one_sample;
vector<QString> names_of_samples;
int length=1;
int probability;
QString tempToTextBrowser;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
       setAcceptDrops(true); // Включаем DnD для MainWindow
        connect(ui->textBrowser_3, &DnDTextBrowser::fileDropped, this, &MainWindow::handleFileDrop);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void remove_non_digits(std::vector<char>& vec) {
    vec.erase(std::remove_if(vec.begin(), vec.end(),
                             [](char c) { return !std::isdigit(static_cast<unsigned char>(c)); }),
              vec.end());
}
void MainWindow::on_pushButton_clicked() {
    result_arr.clear();
    length= (ui->textEdit->toPlainText()).toInt();
    QString formattedText;
       remove_non_digits(zero_file);
    for(int i = 0; i < samples.size(); i++) {
        result_arr.push_back(compare_with_samples_go(zero_file, samples[i], length, probability));
    }

    // Создаем копию zero_file для вставки HTML тегов
    QString zeroFileText;
    for(char c : zero_file) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            zeroFileText += c;
        }
    }

    // Сортируем результаты по индексу в обратном порядке
    // для корректной вставки тегов (избегаем смещения индексов)
    QVector<QPair<int, int>> sortedResults; // Пары (индекс, номер образца)
    for(int i = 0; i < result_arr.size(); i++) {
        if(result_arr[i] != -1) {
            sortedResults.append(qMakePair(result_arr[i], i));
        }
    }
    std::sort(sortedResults.begin(), sortedResults.end(), [](const QPair<int, int> &a, const QPair<int, int> &b) {
        return a.first > b.first;
    });

    // Вставляем HTML теги
    for(int i = 0; i < sortedResults.size(); i++) {
        const auto &result = sortedResults[i];

        // Пропускаем последний элемент
        if (i == sortedResults.size() - 1) {
            continue;
        }

        int index = result.first;
        int sampleIndex = result.second;
        QString redSpanStart = "<span style=\"color:red;\">";
        QString redSpanEnd = "</span>";
        zeroFileText.insert(index + length, redSpanEnd);
        zeroFileText.insert(index, redSpanStart);
    }

    // Отображаем результат в QTextBrowser
    ui->textBrowser_3->clear();
    ui->textBrowser_2->clear();
    tempToTextBrowser=zeroFileText;
    ui->textBrowser_3->setHtml(zeroFileText);
    for(int i=0; i<result_arr.size();i++){
         if(result_arr[i]!=-1){
             ui->textBrowser_2->append("\nНайдено совпадение №" + QString::number(i+1) + ": в сигнале начиная с символа №" + QString::number(result_arr[i]) + "\n" + "Найденая последовательность - "+(names_of_samples[i]));
             ui->textBrowser_2->append("Последовательность:");
            QString temp="";
            vector<char> temp_arr = samples[i];
            for(int j=0; j<temp_arr.size();j++){
                temp+=temp_arr[j];
            }
            QString redSpanStart = "<span style=\"color:red;\">";
            QString redSpanEnd = "</span>";
            for (int index : result_arr) {
                   if (index + length <= zero_file.size()) {
                       QString toFind;
                       for (int j = index; j < index + length; ++j) {
                           toFind += zero_file[j];
                       }

                       int foundIndex = temp.indexOf(toFind);
                       if (foundIndex != -1) {
                           temp.insert(foundIndex + length, redSpanEnd);
                           temp.insert(foundIndex, redSpanStart);
                       }
                   }
               }
            ui->textBrowser_2->append(temp);
         }
     }
}
void MainWindow::handleFileDrop(const QString &filePath) {
    QString temp = "";
    int a = 1;
   input_file inp_file(filePath.toStdString());
   zero_file = inp_file.readAll();
 remove_non_digits(zero_file);
   for(int i = 0; i < (int) zero_file.size(); i++){
       temp+=zero_file[i];
       //temp.remove(temp.size()-1);
       if ((a%64==0)&&(a!=0)){
         ui->textBrowser_3->append(temp);
         temp="";
       }
       a++;
   }
  // MainWindow::on_textEdit_3_textChanged();
   // Получаем директорию исполняемого файла
       QString exeDir = QCoreApplication::applicationDirPath();

       // Строим относительный путь к XML-файлу
       QString xmlFilePath = exeDir + "/samples.xml";

       // Конвертируем QString в std::string (если нужно)
       std::string xmlFilePathStd = xmlFilePath.toStdString();
       // Загружаем XML-файл с помощью PugiXML
          pugi::xml_document doc;
          pugi::xml_parse_result result = doc.load_file(xmlFilePathStd.c_str());

      if (!result) {
          // Обработка ошибки
          system_error();
         ui->textBrowser_3->append(result.description());
      }
      // Получаем корневой элемент
      pugi::xml_node samplesNode = doc.child("samples");

      for (pugi::xml_node mil : samplesNode.children()) {
          std::string content = mil.child_value();
          std::vector<char> temp2(content.begin(), content.end());
          samples.push_back(temp2);
      }
      for (pugi::xml_node mil : samplesNode.children()) {
          // Получаем имя узла и сохраняем его в вектор
          QString name = QString::fromStdString(mil.name());
          names_of_samples.push_back(name);
      }
      for (int k =0;  k < samples.size(); k++){
          a=1;
            std::vector<char> temp2 = samples[k];

      }
}
void MainWindow::on_textBrowser_anchorClicked(const QUrl &arg1)
{
    // Реализация метода, например, обработка нажатия на ссылку в QTextBrowser.
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    // Реализация метода, например, обработка изменения выбранного элемента в QListWidget.
}

void remove_all_nl(std::vector<char>& arr) {
    // std::remove shifts all elements that are not to be removed to the beginning
    // of the range and returns an iterator to the new end of the range
    auto new_end = std::remove(arr.begin(), arr.end(), '\n');

    // Erases all the elements between the new end and the old end,
    // effectively resizing the vector and removing the unwanted characters
    arr.erase(new_end, arr.end());
}










void MainWindow::on_textEdit_textChanged()
{
    bool ok;
    length = ui->textEdit->toPlainText().toInt(&ok);
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    probability=100-value;
    ui->label->setText((QString::number(value))+"%");
}


void MainWindow::on_pushButton_2_clicked()
{
    QString temp = ui->textEdit_2->toPlainText();  // Получаем текст из QTextEdit
    one_sample.clear();
    one_sample.reserve(temp.length());  // Резервируем необходимое количество места в векторе

    for (int i = 0; i < temp.length(); ++i) {
        one_sample.push_back(temp[i].toLatin1());  // Преобразуем QChar в char
    }
    ui->textBrowser_2->clear();
    result_arr.clear();
    result_arr.push_back(compare_with_samples_one_one(zero_file, one_sample));
    if(result_arr[0]!=-1){
        ui->textBrowser_2->append("\nНайдено совпадение в сигнале начиная с символа №" + QString::number(result_arr[0]) + "\n"  );
        QString temp="";
        ui->textBrowser_2->append("Последовательность:");
       vector<char> temp_arr = one_sample;
       for(int j=0; j<temp_arr.size();j++){
           temp+=temp_arr[j];
       }
       ui->textBrowser_2->append(temp);
    }
}
void processText(DnDTextBrowser *textBrowser, int desiredLength) {
    // Получаем текущий текст из textBrowser
    QString text = textBrowser->toPlainText();

    // Удаляем все символы переноса строки
    text.replace("\n", "");

    // Добавляем новые переносы строк в соответствии с desiredLength
    QString processedText;
    for (int i = 0; i < text.length(); i += desiredLength) {
        processedText += text.mid(i, desiredLength) + "\n";
    }

    // Устанавливаем обработанный текст обратно в textBrowser
    textBrowser->setText(processedText);
}


void MainWindow::on_textEdit_3_textChanged() {
    QString text = ui->textEdit_3->toPlainText();

    // Проверяем, что текст не пустой
    if (text.isEmpty()) {
        return;
    }

    // Проверяем, что текст содержит только цифры
    bool isNumber;
    int len = text.toInt(&isNumber);

    if (!isNumber || len <= 0) {
        // Если текст не является числом или число неположительное, не продолжаем
        return;
    }

    // Вызываем функцию processText с корректными параметрами
    processText(ui->textBrowser_3,len);
}

int globaldlina=500;
void MainWindow::on_pushButton_4_clicked()
{


}

