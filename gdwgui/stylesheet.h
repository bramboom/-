#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QString>
class StyleSheet
{
public:
    static QStringList style;
    static const QStringList styleBlue;
    static const QStringList styleGray;
    static const QStringList styleBlueGray;
    static const QStringList stylePink;
    static const QStringList styleBrown;

public:
    StyleSheet();
    static QString GetStyleBtn();
    static QString GetStyleWidget();
    static QString GetLightStyleWidget();
    static QString GetStyleScrollArea();
    static QString GetStyleDatePicker();
    static QString GetStyleComboBox();
    static QString GetDarkStyleWidget();
    static QString GetStyleMenuBar();
    static QString GetStyleAction();
    static void    SetStyleToFile(int styleNumber);
    static void    GetStyleFromFile();
};

#endif // STYLESHEET_H
