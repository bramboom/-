#include "stylesheet.h"
#include <QFile>

using namespace std;

StyleSheet::StyleSheet()
{
}
                                              //default,  light,     dark,      text
const QStringList StyleSheet::styleBlue =     {"#bcd1e0", "#e4e9ed", "#d7dce0", "#000000"};
const QStringList StyleSheet::styleGray =     {"#b5b5b5", "#dbdbdb", "#bfbfbf", "#000000"};
const QStringList StyleSheet::styleBlueGray = {"#287877", "#d4caa0", "#b3bc92", "#000000"};
const QStringList StyleSheet::stylePink =     {"#e0bc6e", "#949c73", "#ffa177", "#000000"};
const QStringList StyleSheet::styleBrown =    {"#1b343c", "#e1b168", "#b96f20", "#ffffff"};

QStringList StyleSheet::style = styleGray;

QString StyleSheet::GetStyleBtn()
{
    return "QPushButton {background-color: " + style[0] + ";"
           "border: 1px solid black;"
           "color: " + style[3] + ";"
           "border-radius: 4px;}";
}

QString StyleSheet::GetStyleWidget()
{
    return "QWidget {background-color: " + style[1] + ";}";
}

QString StyleSheet::GetStyleScrollArea()
{
    return "QScrollArea {background-color: " + style[0] + ";"
            "border: 1px solid black;}";
}

QString StyleSheet::GetStyleDatePicker()
{
    return "QDateTimeEdit {background-color: #ffffff; }"
           "border: 1px solid black;";
}

QString StyleSheet::GetStyleComboBox()
{
    return "QComboBox {background-color: " + style[0] + ";"
           "border: 1px solid black;"
           "color: " + style[3] + ";"
           "border-radius: 4px;}";
}

QString StyleSheet::GetLightStyleWidget()
{
    return "QWidget {background-color: #ffffff; }";
}

QString StyleSheet::GetDarkStyleWidget()
{
    return "QWidget {background-color: " + style[2] + ";}";
}

QString StyleSheet::GetStyleMenuBar()
{
    return "QMenuBar {background-color: " + style[0] + ";"
           "color: " + style[3] + ";}";
}

QString StyleSheet::GetStyleAction()
{
    return "QAction {background-color: " + style[0] + ";"
           "color: " + style[3] + ";}";
}

void StyleSheet::SetStyleToFile(int styleNumber)
{
    QString string;
    switch (styleNumber) {
    case 1:
    {
        string = styleBlue[0] + "\n" +
                 styleBlue[1] + "\n" +
                 styleBlue[2] + "\n" +
                 styleBlue[3] + "\n";
        break;
    }
    case 2:
    {
        string = styleGray[0] + "\n" +
                 styleGray[1] + "\n" +
                 styleGray[2] + "\n" +
                 styleGray[3] + "\n";
        break;
    }
    case 3:
    {
        string = styleBlueGray[0] + "\n" +
                 styleBlueGray[1] + "\n" +
                 styleBlueGray[2] + "\n" +
                 styleBlueGray[3] + "\n";
        break;
    }
    case 4:
    {
        string = stylePink[0] + "\n" +
                 stylePink[1] + "\n" +
                 stylePink[2] + "\n" +
                 stylePink[3] + "\n";
        break;
    }
    default:
    {
        string = styleBrown[0] + "\n" +
                 styleBrown[1] + "\n" +
                 styleBrown[2] + "\n" +
                 styleBrown[3] + "\n";
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
