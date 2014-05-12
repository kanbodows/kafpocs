#include "styles.h"

Styles::Styles()
{
    currentPressedStyle = "font:bold; background: #1C86EE; "\
            "color: white;";

    currentStyle = "QPushButton {"\
            " color: #333;"\
            " border: 2px solid #555;"\
            " border-radius:12px;"\
            " padding: 2px;"\
            " background: qradialgradient(cx: 0.3, cy: -0.4,"\
            " fx: 0.3, fy: -0.4,"\
            " radius: 1.35, stop: 0 #fff, stop: 1 #eee);"\
            " min-width: 80px;"\
            " }"\
            " QPushButton:hover { "\
            " background: #1C86EE;"\
            " color: white;"\
            " }"\
            " QPushButton:pressed {"\
            " background: qradialgradient(cx: 0.4, cy: -0.1,"\
            " fx: 0.4, fy: -0.1,"\
            " radius: 1.35, stop: 0 #fff, stop: 1 #eee);"\
            " color: black;"\
            " }";

    pressedStyleSheet = "QLineEdit:pressed, QListView:pressed{background: white;"\
            "padding: 1px; border-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop: 0 #00B2EE,  stop: 0.3 #00BFFF, stop: 1 #87CEFF);"\
            "border-radius: 8px;}";

    wrongStyleSheet = "background: white;"
            "padding: 1px;"
            "border: 2px solid red;"
            "border-radius: 8px;";

    pressedStyleSheetSum = "QSpinBox:pressed{background: white;"\
            "padding: 1px; border-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop: 0 #00B2EE,  stop: 0.3 #00BFFF, stop: 1 #87CEFF);"\
            "border-radius: 8px;}";

    wrongStyleSheet2 = "background: white;"
            "padding: 1px;"
            "border: 2px solid red;";

    wrongStyleSheet3 = "QComboBox {border: 1px solid red;"
            "border-radius: 3px;"
            "padding: 1px 18px 1px 3px;"
            "min-width: 6em;}";

    pressedStyleSheet3 = "QComboBox{border: 1px solid gray;"
            "border-radius: 3px;"
            "padding: 1px 18px 1px 3px;"
            "min-width: 6em;}";

    activeReport = "color: green; font:bold;";

    inActiveReport = "color:black; font:normal;";

    reportCss = "table.mine{width: 100%;font-size:120px;"
            "border-width: 2px; border-style: solid; border-color: black; border-collapse:collapse;}"
            "td{text-align: center;"
            "vertical-align:middle;"
            "padding-right:3px;padding-left:3px;}"
            "th{padding:3px;vertical-align:middle;text-align: center;color:#666;background:#eaebec; border-color:red;}"
            "td.grHead {background-color:green; text-align:left;"
            "color:white;"
            "padding-left:10px;"
            "padding-right:10px; border-width:2px; border-style:solid; border-color:red;}"
            "td.sum{font-style:bold;}"
            "body { line-height:20%;}"
            ;

}
