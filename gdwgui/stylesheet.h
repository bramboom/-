#ifndef STYLESHEET_H
#define STYLESHEET_H

#include <QString>
class StyleSheet
{
public:
    static QStringList style;
    static const QStringList styleBlue;
    static const QStringList styleGray;

public:
    StyleSheet();
    static QString GetBlueStyleBtn();
    static QString GetBlueStyleWidget();
    static QString GetBlueLightStyleWidget();
    static QString GetBlueStyleScrollArea();
    static QString GetBlueStyleDatePicker();
    static QString GetBlueStyleComboBox();
    static QString GetBlueDarkStyleWidget();
    static QString GetBlueStyleMenuBar();
    static void    SetStyleToFile(int styleNumber);
    static void    GetStyleFromFile();
};

#endif // STYLESHEET_H
