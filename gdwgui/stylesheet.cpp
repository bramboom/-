#include "stylesheet.h"
#include <QFile>

StyleSheet::StyleSheet()
{
}

const QStringList StyleSheet::styleBlue = {"#bcd1e0", "#e4e9ed", "#d7dce0"};
const QStringList StyleSheet::styleGray = {"#b5b5b5", "#dbdbdb", "#bfbfbf"};
QStringList StyleSheet::style = styleGray;

QString StyleSheet::GetBlueStyleBtn()
{
    return "QPushButton {background-color: " + style[0] + ";"
           "border: 1px solid black;"
           "border-radius: 4px;}";
}

QString StyleSheet::GetBlueStyleWidget()
{
    return "QWidget {background-color: " + style[1] + ";";
}

QString StyleSheet::GetBlueStyleScrollArea()
{
    return "QScrollArea {background-color: " + style[0] + ";"
            "border: 1px solid black;";
}

QString StyleSheet::GetBlueStyleDatePicker()
{
    return "QDateTimeEdit {background-color: #ffffff; }"
           "border: 1px solid black;";
}

QString StyleSheet::GetBlueStyleComboBox()
{
    return "QComboBox {background-color: " + style[0] + ";"
           "border: 1px solid black;"
           "border-radius: 4px;}";
}

QString StyleSheet::GetBlueLightStyleWidget()
{
    return "QWidget {background-color: #ffffff; }";
}

QString StyleSheet::GetBlueDarkStyleWidget()
{
    return "QWidget {background-color: " + style[2] + ";";
}

QString StyleSheet::GetBlueStyleMenuBar()
{
   return "QMenuBar {background-color: " + style[0] + ";";
}

void StyleSheet::SetStyleToFile(int styleNumber)
{
    QString string;
    switch (styleNumber) {
    case 1:
    {
        string = styleBlue[0] + "\n" +
                 styleBlue[1] + "\n" +
                 styleBlue[2] + "\n";
        break;
    }
    //case 2:
    //{
    //    string = styleGray[0] + "/n" +
    //             styleGray[1] + "/n" +
    //             styleGray[2] + "/n";
    //    break;
    //}
    default:
    {
        string = styleGray[0] + "\n" +
                 styleGray[1] + "\n" +
                 styleGray[2] + "\n";
        break;
    }
    }
    QFile file("../Style.txt");
    if ((file.exists())&&(file.open(QIODevice::WriteOnly)))
    {
        file.write(string.toUtf8());
        file.close();
    }
}

void StyleSheet::GetStyleFromFile()
{
    QFile file("../Style.txt");

    if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
    {
        for(int index = 0; !file.atEnd(); index++)
        {
            style[index] = file.readLine();
        }
        file.close();
    }
}
