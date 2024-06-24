/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#include <QtCore/QQueue>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[218];
    char stringdata0[4301];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 16), // "sig_updateBuffer"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 14), // "unsigned char*"
QT_MOC_LITERAL(4, 44, 7), // "pbuffer"
QT_MOC_LITERAL(5, 52, 20), // "imageProcessedFinish"
QT_MOC_LITERAL(6, 73, 20), // "startImageProcessing"
QT_MOC_LITERAL(7, 94, 11), // "cameraWidth"
QT_MOC_LITERAL(8, 106, 12), // "cameraHeight"
QT_MOC_LITERAL(9, 119, 11), // "std::string"
QT_MOC_LITERAL(10, 131, 8), // "appDirCv"
QT_MOC_LITERAL(11, 140, 11), // "freeCollect"
QT_MOC_LITERAL(12, 152, 11), // "picNumCount"
QT_MOC_LITERAL(13, 164, 24), // "onCheckBox_Ori_2_changed"
QT_MOC_LITERAL(14, 189, 22), // "onCheckBox_Ori_changed"
QT_MOC_LITERAL(15, 212, 23), // "onPushButton_11_clicked"
QT_MOC_LITERAL(16, 236, 16), // "onTabIndexChange"
QT_MOC_LITERAL(17, 253, 5), // "index"
QT_MOC_LITERAL(18, 259, 24), // "on_insName_4_textChanged"
QT_MOC_LITERAL(19, 284, 14), // "process_buffer"
QT_MOC_LITERAL(20, 299, 21), // "lineEdit_8returnPress"
QT_MOC_LITERAL(21, 321, 14), // "ReturnToCenter"
QT_MOC_LITERAL(22, 336, 22), // "checkCheckedCheckBoxes"
QT_MOC_LITERAL(23, 359, 18), // "QQueue<QCheckBox*>"
QT_MOC_LITERAL(24, 378, 17), // "checkedCheckBoxes"
QT_MOC_LITERAL(25, 396, 13), // "SetNormalGain"
QT_MOC_LITERAL(26, 410, 15), // "SetSpecularGain"
QT_MOC_LITERAL(27, 426, 14), // "SetDiffuseGain"
QT_MOC_LITERAL(28, 441, 12), // "SetShapeGain"
QT_MOC_LITERAL(29, 454, 15), // "DirectionChange"
QT_MOC_LITERAL(30, 470, 27), // "on_DirectionPoseBtn_clicked"
QT_MOC_LITERAL(31, 498, 9), // "LanChange"
QT_MOC_LITERAL(32, 508, 10), // "LanChange2"
QT_MOC_LITERAL(33, 519, 18), // "ABcheckboxConverse"
QT_MOC_LITERAL(34, 538, 17), // "DeviceInfoDisplay"
QT_MOC_LITERAL(35, 556, 10), // "CameraInfo"
QT_MOC_LITERAL(36, 567, 9), // "LightInfo"
QT_MOC_LITERAL(37, 577, 21), // "handleDeviceSelection"
QT_MOC_LITERAL(38, 599, 11), // "lightHandle"
QT_MOC_LITERAL(39, 611, 12), // "cameraHandle"
QT_MOC_LITERAL(40, 624, 12), // "systemHandle"
QT_MOC_LITERAL(41, 637, 16), // "slot_sendPercent"
QT_MOC_LITERAL(42, 654, 14), // "ReadUpgradInfo"
QT_MOC_LITERAL(43, 669, 9), // "InsUpgrad"
QT_MOC_LITERAL(44, 679, 15), // "WriteUpgradInfo"
QT_MOC_LITERAL(45, 695, 4), // "info"
QT_MOC_LITERAL(46, 700, 17), // "ArrayToVariable32"
QT_MOC_LITERAL(47, 718, 8), // "uint32_t"
QT_MOC_LITERAL(48, 727, 8), // "uint8_t*"
QT_MOC_LITERAL(49, 736, 5), // "array"
QT_MOC_LITERAL(50, 742, 7), // "uint8_t"
QT_MOC_LITERAL(51, 750, 6), // "length"
QT_MOC_LITERAL(52, 757, 13), // "FormatStrData"
QT_MOC_LITERAL(53, 771, 12), // "std::string*"
QT_MOC_LITERAL(54, 784, 4), // "aStr"
QT_MOC_LITERAL(55, 789, 7), // "aStrLen"
QT_MOC_LITERAL(56, 797, 33), // "on_comboBox_7_currentIndexCha..."
QT_MOC_LITERAL(57, 831, 19), // "generateSystemModel"
QT_MOC_LITERAL(58, 851, 11), // "cameraModel"
QT_MOC_LITERAL(59, 863, 16), // "lightSourceModel"
QT_MOC_LITERAL(60, 880, 9), // "ReadPose2"
QT_MOC_LITERAL(61, 890, 11), // "ABconverse2"
QT_MOC_LITERAL(62, 902, 12), // "ABencodePose"
QT_MOC_LITERAL(63, 915, 10), // "ABconverse"
QT_MOC_LITERAL(64, 926, 8), // "ReadPose"
QT_MOC_LITERAL(65, 935, 14), // "writeRegisters"
QT_MOC_LITERAL(66, 950, 10), // "ReadCamTem"
QT_MOC_LITERAL(67, 961, 12), // "ReadBoardTem"
QT_MOC_LITERAL(68, 974, 11), // "ReadFpgaTem"
QT_MOC_LITERAL(69, 986, 13), // "DevIp_clicked"
QT_MOC_LITERAL(70, 1000, 12), // "updateImages"
QT_MOC_LITERAL(71, 1013, 20), // "simulateUpdateBuffer"
QT_MOC_LITERAL(72, 1034, 12), // "updateValues"
QT_MOC_LITERAL(73, 1047, 6), // "normal"
QT_MOC_LITERAL(74, 1054, 8), // "specular"
QT_MOC_LITERAL(75, 1063, 7), // "diffuse"
QT_MOC_LITERAL(76, 1071, 5), // "shape"
QT_MOC_LITERAL(77, 1077, 18), // "ImageValue_clicked"
QT_MOC_LITERAL(78, 1096, 16), // "PoseBtn2_clicked"
QT_MOC_LITERAL(79, 1113, 6), // "showXY"
QT_MOC_LITERAL(80, 1120, 1), // "x"
QT_MOC_LITERAL(81, 1122, 1), // "y"
QT_MOC_LITERAL(82, 1124, 9), // "grayvalue"
QT_MOC_LITERAL(83, 1134, 7), // "showXY2"
QT_MOC_LITERAL(84, 1142, 7), // "showXY3"
QT_MOC_LITERAL(85, 1150, 15), // "PoseBtn_clicked"
QT_MOC_LITERAL(86, 1166, 17), // "RegionBtn_clicked"
QT_MOC_LITERAL(87, 1184, 13), // "copyImageData"
QT_MOC_LITERAL(88, 1198, 8), // "Data_25D"
QT_MOC_LITERAL(89, 1207, 8), // "CamImage"
QT_MOC_LITERAL(90, 1216, 18), // "l_image_show_width"
QT_MOC_LITERAL(91, 1235, 19), // "l_image_show_height"
QT_MOC_LITERAL(92, 1255, 24), // "on_FreLine_returnPressed"
QT_MOC_LITERAL(93, 1280, 33), // "on_TriggerFre_currentIndexCha..."
QT_MOC_LITERAL(94, 1314, 32), // "on_LightMode_currentIndexChanged"
QT_MOC_LITERAL(95, 1347, 20), // "slot_btnOffsetMethod"
QT_MOC_LITERAL(96, 1368, 11), // "on_LightBtn"
QT_MOC_LITERAL(97, 1380, 22), // "countCheckedCheckBoxes"
QT_MOC_LITERAL(98, 1403, 17), // "onCheckBoxChanged"
QT_MOC_LITERAL(99, 1421, 5), // "state"
QT_MOC_LITERAL(100, 1427, 9), // "showImage"
QT_MOC_LITERAL(101, 1437, 8), // "filePath"
QT_MOC_LITERAL(102, 1446, 4), // "text"
QT_MOC_LITERAL(103, 1451, 9), // "viewIndex"
QT_MOC_LITERAL(104, 1461, 17), // "onComboBoxChanged"
QT_MOC_LITERAL(105, 1479, 10), // "clearImage"
QT_MOC_LITERAL(106, 1490, 21), // "clearAllGraphicsViews"
QT_MOC_LITERAL(107, 1512, 22), // "processAndDisplayImage"
QT_MOC_LITERAL(108, 1535, 9), // "slot_Comm"
QT_MOC_LITERAL(109, 1545, 15), // "slot_DeviceInfo"
QT_MOC_LITERAL(110, 1561, 14), // "slot_LightEdit"
QT_MOC_LITERAL(111, 1576, 13), // "slot_MainPage"
QT_MOC_LITERAL(112, 1590, 10), // "InitWindow"
QT_MOC_LITERAL(113, 1601, 13), // "update_buffer"
QT_MOC_LITERAL(114, 1615, 15), // "InsPMDL_Buffer*"
QT_MOC_LITERAL(115, 1631, 17), // "slot_updateBuffer"
QT_MOC_LITERAL(116, 1649, 17), // "addToStateMessage"
QT_MOC_LITERAL(117, 1667, 3), // "str"
QT_MOC_LITERAL(118, 1671, 5), // "Error"
QT_MOC_LITERAL(119, 1677, 24), // "modbus_state_change_slot"
QT_MOC_LITERAL(120, 1702, 20), // "QModbusDevice::State"
QT_MOC_LITERAL(121, 1723, 26), // "modbus_error_occurred_slot"
QT_MOC_LITERAL(122, 1750, 20), // "QModbusDevice::Error"
QT_MOC_LITERAL(123, 1771, 21), // "modbus_read_over_slot"
QT_MOC_LITERAL(124, 1793, 29), // "QModbusDataUnit::RegisterType"
QT_MOC_LITERAL(125, 1823, 4), // "type"
QT_MOC_LITERAL(126, 1828, 5), // "isErr"
QT_MOC_LITERAL(127, 1834, 16), // "QVector<quint16>"
QT_MOC_LITERAL(128, 1851, 8), // "readData"
QT_MOC_LITERAL(129, 1860, 16), // "modbusPeriodRead"
QT_MOC_LITERAL(130, 1877, 9), // "readReady"
QT_MOC_LITERAL(131, 1887, 15), // "slot_infoUpdate"
QT_MOC_LITERAL(132, 1903, 23), // "on_StateSaveBtn_clicked"
QT_MOC_LITERAL(133, 1927, 24), // "on_StateClearBtn_clicked"
QT_MOC_LITERAL(134, 1952, 21), // "on_toolButton_clicked"
QT_MOC_LITERAL(135, 1974, 23), // "on_toolButton_2_clicked"
QT_MOC_LITERAL(136, 1998, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(137, 2020, 23), // "on_pushButton_2_clicked"
QT_MOC_LITERAL(138, 2044, 23), // "on_toolButton_3_clicked"
QT_MOC_LITERAL(139, 2068, 23), // "on_toolButton_4_clicked"
QT_MOC_LITERAL(140, 2092, 21), // "on_connectBtn_clicked"
QT_MOC_LITERAL(141, 2114, 22), // "on_startAcqBtn_clicked"
QT_MOC_LITERAL(142, 2137, 21), // "on_stopAcqBtn_clicked"
QT_MOC_LITERAL(143, 2159, 24), // "on_disConnectBtn_clicked"
QT_MOC_LITERAL(144, 2184, 19), // "on_checkBox_clicked"
QT_MOC_LITERAL(145, 2204, 7), // "checked"
QT_MOC_LITERAL(146, 2212, 31), // "on_doubleSpinBox_3_valueChanged"
QT_MOC_LITERAL(147, 2244, 4), // "arg1"
QT_MOC_LITERAL(148, 2249, 34), // "on_ChuFaCombox_currentIndexCh..."
QT_MOC_LITERAL(149, 2284, 29), // "on_BeiPinSpinBox_valueChanged"
QT_MOC_LITERAL(150, 2314, 29), // "on_FenPinSpinBox_valueChanged"
QT_MOC_LITERAL(151, 2344, 29), // "on_ModbusClientBtn_10_clicked"
QT_MOC_LITERAL(152, 2374, 21), // "on_ClearBtn_2_clicked"
QT_MOC_LITERAL(153, 2396, 38), // "on_EnCodeTriComBox_currentInd..."
QT_MOC_LITERAL(154, 2435, 34), // "on_LightMode_2_currentIndexCh..."
QT_MOC_LITERAL(155, 2470, 33), // "on_comboBox_3_currentIndexCha..."
QT_MOC_LITERAL(156, 2504, 29), // "on_ModbusClientBtn_11_clicked"
QT_MOC_LITERAL(157, 2534, 21), // "on_ClearBtn_3_clicked"
QT_MOC_LITERAL(158, 2556, 31), // "on_insInterface_5_returnPressed"
QT_MOC_LITERAL(159, 2588, 31), // "on_insInterface_8_returnPressed"
QT_MOC_LITERAL(160, 2620, 31), // "on_insInterface_6_returnPressed"
QT_MOC_LITERAL(161, 2652, 33), // "on_comboBox_4_currentIndexCha..."
QT_MOC_LITERAL(162, 2686, 23), // "on_pushButton_5_clicked"
QT_MOC_LITERAL(163, 2710, 23), // "on_pushButton_6_clicked"
QT_MOC_LITERAL(164, 2734, 23), // "on_pushButton_7_clicked"
QT_MOC_LITERAL(165, 2758, 23), // "on_pushButton_3_clicked"
QT_MOC_LITERAL(166, 2782, 27), // "on_lineEdit_9_returnPressed"
QT_MOC_LITERAL(167, 2810, 28), // "on_lineEdit_10_returnPressed"
QT_MOC_LITERAL(168, 2839, 27), // "on_lineEdit_7_returnPressed"
QT_MOC_LITERAL(169, 2867, 25), // "on_insName_10_textChanged"
QT_MOC_LITERAL(170, 2893, 24), // "on_ExNumUI_returnPressed"
QT_MOC_LITERAL(171, 2918, 36), // "on_ChuFaCombox_2_currentIndex..."
QT_MOC_LITERAL(172, 2955, 26), // "on_ExDelayUI_returnPressed"
QT_MOC_LITERAL(173, 2982, 26), // "on_ExWidthUI_returnPressed"
QT_MOC_LITERAL(174, 3009, 25), // "on_CamTriUI_returnPressed"
QT_MOC_LITERAL(175, 3035, 27), // "on_lineEdit_8_returnPressed"
QT_MOC_LITERAL(176, 3063, 28), // "on_lineEdit_11_returnPressed"
QT_MOC_LITERAL(177, 3092, 34), // "on_LightMode_5_currentIndexCh..."
QT_MOC_LITERAL(178, 3127, 34), // "on_LightMode_4_currentIndexCh..."
QT_MOC_LITERAL(179, 3162, 34), // "on_LightMode_6_currentIndexCh..."
QT_MOC_LITERAL(180, 3197, 13), // "loadConfigure"
QT_MOC_LITERAL(181, 3211, 13), // "saveConfigure"
QT_MOC_LITERAL(182, 3225, 18), // "saveConfigureFlash"
QT_MOC_LITERAL(183, 3244, 30), // "on_checkBox_SP2_2_stateChanged"
QT_MOC_LITERAL(184, 3275, 29), // "on_insInterface_4_textChanged"
QT_MOC_LITERAL(185, 3305, 28), // "on_lineEdit_12_returnPressed"
QT_MOC_LITERAL(186, 3334, 33), // "on_LightMode3_currentIndexCha..."
QT_MOC_LITERAL(187, 3368, 30), // "on_checkBox_SP2_3_stateChanged"
QT_MOC_LITERAL(188, 3399, 28), // "on_lineEdit_18_returnPressed"
QT_MOC_LITERAL(189, 3428, 34), // "on_LightMode_3_currentIndexCh..."
QT_MOC_LITERAL(190, 3463, 28), // "on_lineEdit_13_returnPressed"
QT_MOC_LITERAL(191, 3492, 28), // "on_lineEdit_16_returnPressed"
QT_MOC_LITERAL(192, 3521, 28), // "on_lineEdit_15_returnPressed"
QT_MOC_LITERAL(193, 3550, 28), // "on_lineEdit_17_returnPressed"
QT_MOC_LITERAL(194, 3579, 28), // "on_lineEdit_14_returnPressed"
QT_MOC_LITERAL(195, 3608, 34), // "on_LightMode_7_currentIndexCh..."
QT_MOC_LITERAL(196, 3643, 28), // "on_lineEdit_23_returnPressed"
QT_MOC_LITERAL(197, 3672, 36), // "on_ChuFaCombox_3_currentIndex..."
QT_MOC_LITERAL(198, 3709, 31), // "on_FenPinSpinBox_2_valueChanged"
QT_MOC_LITERAL(199, 3741, 31), // "on_BeiPinSpinBox_2_valueChanged"
QT_MOC_LITERAL(200, 3773, 28), // "on_lineEdit_24_returnPressed"
QT_MOC_LITERAL(201, 3802, 28), // "on_lineEdit_25_returnPressed"
QT_MOC_LITERAL(202, 3831, 34), // "on_LightMode_8_currentIndexCh..."
QT_MOC_LITERAL(203, 3866, 28), // "on_lineEdit_26_returnPressed"
QT_MOC_LITERAL(204, 3895, 28), // "on_lineEdit_27_returnPressed"
QT_MOC_LITERAL(205, 3924, 28), // "on_lineEdit_28_returnPressed"
QT_MOC_LITERAL(206, 3953, 28), // "on_lineEdit_29_returnPressed"
QT_MOC_LITERAL(207, 3982, 28), // "on_lineEdit_30_returnPressed"
QT_MOC_LITERAL(208, 4011, 35), // "on_LightMode_14_currentIndexC..."
QT_MOC_LITERAL(209, 4047, 35), // "on_LightMode_12_currentIndexC..."
QT_MOC_LITERAL(210, 4083, 23), // "on_pushButton_4_clicked"
QT_MOC_LITERAL(211, 4107, 33), // "on_comboBox_6_currentIndexCha..."
QT_MOC_LITERAL(212, 4141, 28), // "on_lineEdit_19_returnPressed"
QT_MOC_LITERAL(213, 4170, 28), // "on_lineEdit_20_returnPressed"
QT_MOC_LITERAL(214, 4199, 28), // "on_lineEdit_21_returnPressed"
QT_MOC_LITERAL(215, 4228, 23), // "on_pushButton_8_clicked"
QT_MOC_LITERAL(216, 4252, 23), // "on_pushButton_9_clicked"
QT_MOC_LITERAL(217, 4276, 24) // "on_pushButton_10_clicked"

    },
    "MainWindow\0sig_updateBuffer\0\0"
    "unsigned char*\0pbuffer\0imageProcessedFinish\0"
    "startImageProcessing\0cameraWidth\0"
    "cameraHeight\0std::string\0appDirCv\0"
    "freeCollect\0picNumCount\0"
    "onCheckBox_Ori_2_changed\0"
    "onCheckBox_Ori_changed\0onPushButton_11_clicked\0"
    "onTabIndexChange\0index\0on_insName_4_textChanged\0"
    "process_buffer\0lineEdit_8returnPress\0"
    "ReturnToCenter\0checkCheckedCheckBoxes\0"
    "QQueue<QCheckBox*>\0checkedCheckBoxes\0"
    "SetNormalGain\0SetSpecularGain\0"
    "SetDiffuseGain\0SetShapeGain\0DirectionChange\0"
    "on_DirectionPoseBtn_clicked\0LanChange\0"
    "LanChange2\0ABcheckboxConverse\0"
    "DeviceInfoDisplay\0CameraInfo\0LightInfo\0"
    "handleDeviceSelection\0lightHandle\0"
    "cameraHandle\0systemHandle\0slot_sendPercent\0"
    "ReadUpgradInfo\0InsUpgrad\0WriteUpgradInfo\0"
    "info\0ArrayToVariable32\0uint32_t\0"
    "uint8_t*\0array\0uint8_t\0length\0"
    "FormatStrData\0std::string*\0aStr\0aStrLen\0"
    "on_comboBox_7_currentIndexChanged\0"
    "generateSystemModel\0cameraModel\0"
    "lightSourceModel\0ReadPose2\0ABconverse2\0"
    "ABencodePose\0ABconverse\0ReadPose\0"
    "writeRegisters\0ReadCamTem\0ReadBoardTem\0"
    "ReadFpgaTem\0DevIp_clicked\0updateImages\0"
    "simulateUpdateBuffer\0updateValues\0"
    "normal\0specular\0diffuse\0shape\0"
    "ImageValue_clicked\0PoseBtn2_clicked\0"
    "showXY\0x\0y\0grayvalue\0showXY2\0showXY3\0"
    "PoseBtn_clicked\0RegionBtn_clicked\0"
    "copyImageData\0Data_25D\0CamImage\0"
    "l_image_show_width\0l_image_show_height\0"
    "on_FreLine_returnPressed\0"
    "on_TriggerFre_currentIndexChanged\0"
    "on_LightMode_currentIndexChanged\0"
    "slot_btnOffsetMethod\0on_LightBtn\0"
    "countCheckedCheckBoxes\0onCheckBoxChanged\0"
    "state\0showImage\0filePath\0text\0viewIndex\0"
    "onComboBoxChanged\0clearImage\0"
    "clearAllGraphicsViews\0processAndDisplayImage\0"
    "slot_Comm\0slot_DeviceInfo\0slot_LightEdit\0"
    "slot_MainPage\0InitWindow\0update_buffer\0"
    "InsPMDL_Buffer*\0slot_updateBuffer\0"
    "addToStateMessage\0str\0Error\0"
    "modbus_state_change_slot\0QModbusDevice::State\0"
    "modbus_error_occurred_slot\0"
    "QModbusDevice::Error\0modbus_read_over_slot\0"
    "QModbusDataUnit::RegisterType\0type\0"
    "isErr\0QVector<quint16>\0readData\0"
    "modbusPeriodRead\0readReady\0slot_infoUpdate\0"
    "on_StateSaveBtn_clicked\0"
    "on_StateClearBtn_clicked\0on_toolButton_clicked\0"
    "on_toolButton_2_clicked\0on_pushButton_clicked\0"
    "on_pushButton_2_clicked\0on_toolButton_3_clicked\0"
    "on_toolButton_4_clicked\0on_connectBtn_clicked\0"
    "on_startAcqBtn_clicked\0on_stopAcqBtn_clicked\0"
    "on_disConnectBtn_clicked\0on_checkBox_clicked\0"
    "checked\0on_doubleSpinBox_3_valueChanged\0"
    "arg1\0on_ChuFaCombox_currentIndexChanged\0"
    "on_BeiPinSpinBox_valueChanged\0"
    "on_FenPinSpinBox_valueChanged\0"
    "on_ModbusClientBtn_10_clicked\0"
    "on_ClearBtn_2_clicked\0"
    "on_EnCodeTriComBox_currentIndexChanged\0"
    "on_LightMode_2_currentIndexChanged\0"
    "on_comboBox_3_currentIndexChanged\0"
    "on_ModbusClientBtn_11_clicked\0"
    "on_ClearBtn_3_clicked\0"
    "on_insInterface_5_returnPressed\0"
    "on_insInterface_8_returnPressed\0"
    "on_insInterface_6_returnPressed\0"
    "on_comboBox_4_currentIndexChanged\0"
    "on_pushButton_5_clicked\0on_pushButton_6_clicked\0"
    "on_pushButton_7_clicked\0on_pushButton_3_clicked\0"
    "on_lineEdit_9_returnPressed\0"
    "on_lineEdit_10_returnPressed\0"
    "on_lineEdit_7_returnPressed\0"
    "on_insName_10_textChanged\0"
    "on_ExNumUI_returnPressed\0"
    "on_ChuFaCombox_2_currentIndexChanged\0"
    "on_ExDelayUI_returnPressed\0"
    "on_ExWidthUI_returnPressed\0"
    "on_CamTriUI_returnPressed\0"
    "on_lineEdit_8_returnPressed\0"
    "on_lineEdit_11_returnPressed\0"
    "on_LightMode_5_currentIndexChanged\0"
    "on_LightMode_4_currentIndexChanged\0"
    "on_LightMode_6_currentIndexChanged\0"
    "loadConfigure\0saveConfigure\0"
    "saveConfigureFlash\0on_checkBox_SP2_2_stateChanged\0"
    "on_insInterface_4_textChanged\0"
    "on_lineEdit_12_returnPressed\0"
    "on_LightMode3_currentIndexChanged\0"
    "on_checkBox_SP2_3_stateChanged\0"
    "on_lineEdit_18_returnPressed\0"
    "on_LightMode_3_currentIndexChanged\0"
    "on_lineEdit_13_returnPressed\0"
    "on_lineEdit_16_returnPressed\0"
    "on_lineEdit_15_returnPressed\0"
    "on_lineEdit_17_returnPressed\0"
    "on_lineEdit_14_returnPressed\0"
    "on_LightMode_7_currentIndexChanged\0"
    "on_lineEdit_23_returnPressed\0"
    "on_ChuFaCombox_3_currentIndexChanged\0"
    "on_FenPinSpinBox_2_valueChanged\0"
    "on_BeiPinSpinBox_2_valueChanged\0"
    "on_lineEdit_24_returnPressed\0"
    "on_lineEdit_25_returnPressed\0"
    "on_LightMode_8_currentIndexChanged\0"
    "on_lineEdit_26_returnPressed\0"
    "on_lineEdit_27_returnPressed\0"
    "on_lineEdit_28_returnPressed\0"
    "on_lineEdit_29_returnPressed\0"
    "on_lineEdit_30_returnPressed\0"
    "on_LightMode_14_currentIndexChanged\0"
    "on_LightMode_12_currentIndexChanged\0"
    "on_pushButton_4_clicked\0"
    "on_comboBox_6_currentIndexChanged\0"
    "on_lineEdit_19_returnPressed\0"
    "on_lineEdit_20_returnPressed\0"
    "on_lineEdit_21_returnPressed\0"
    "on_pushButton_8_clicked\0on_pushButton_9_clicked\0"
    "on_pushButton_10_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
     161,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  819,    2, 0x06 /* Public */,
       5,    0,  822,    2, 0x06 /* Public */,
       6,    6,  823,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,  836,    2, 0x0a /* Public */,
      14,    0,  837,    2, 0x0a /* Public */,
      15,    0,  838,    2, 0x0a /* Public */,
      16,    1,  839,    2, 0x0a /* Public */,
      18,    0,  842,    2, 0x0a /* Public */,
      19,    1,  843,    2, 0x0a /* Public */,
      20,    0,  846,    2, 0x0a /* Public */,
      21,    0,  847,    2, 0x0a /* Public */,
      22,    1,  848,    2, 0x0a /* Public */,
      25,    0,  851,    2, 0x0a /* Public */,
      26,    0,  852,    2, 0x0a /* Public */,
      27,    0,  853,    2, 0x0a /* Public */,
      28,    0,  854,    2, 0x0a /* Public */,
      29,    1,  855,    2, 0x0a /* Public */,
      30,    0,  858,    2, 0x0a /* Public */,
      31,    0,  859,    2, 0x0a /* Public */,
      32,    0,  860,    2, 0x0a /* Public */,
      33,    0,  861,    2, 0x0a /* Public */,
      34,    2,  862,    2, 0x0a /* Public */,
      37,    3,  867,    2, 0x0a /* Public */,
      41,    0,  874,    2, 0x0a /* Public */,
      42,    0,  875,    2, 0x0a /* Public */,
      44,    1,  876,    2, 0x0a /* Public */,
      46,    2,  879,    2, 0x0a /* Public */,
      52,    2,  884,    2, 0x0a /* Public */,
      56,    1,  889,    2, 0x0a /* Public */,
      57,    2,  892,    2, 0x0a /* Public */,
      60,    0,  897,    2, 0x0a /* Public */,
      61,    0,  898,    2, 0x0a /* Public */,
      62,    0,  899,    2, 0x0a /* Public */,
      63,    0,  900,    2, 0x0a /* Public */,
      64,    0,  901,    2, 0x0a /* Public */,
      65,    0,  902,    2, 0x0a /* Public */,
      66,    0,  903,    2, 0x0a /* Public */,
      67,    0,  904,    2, 0x0a /* Public */,
      68,    0,  905,    2, 0x0a /* Public */,
      69,    0,  906,    2, 0x0a /* Public */,
      70,    0,  907,    2, 0x0a /* Public */,
      71,    0,  908,    2, 0x0a /* Public */,
      72,    4,  909,    2, 0x0a /* Public */,
      77,    0,  918,    2, 0x0a /* Public */,
      78,    0,  919,    2, 0x0a /* Public */,
      79,    3,  920,    2, 0x0a /* Public */,
      83,    3,  927,    2, 0x0a /* Public */,
      84,    3,  934,    2, 0x0a /* Public */,
      85,    0,  941,    2, 0x0a /* Public */,
      86,    0,  942,    2, 0x0a /* Public */,
      87,    3,  943,    2, 0x0a /* Public */,
      92,    0,  950,    2, 0x0a /* Public */,
      93,    1,  951,    2, 0x0a /* Public */,
      94,    1,  954,    2, 0x0a /* Public */,
      95,    0,  957,    2, 0x0a /* Public */,
      96,    0,  958,    2, 0x0a /* Public */,
      97,    0,  959,    2, 0x0a /* Public */,
      98,    1,  960,    2, 0x0a /* Public */,
     100,    3,  963,    2, 0x0a /* Public */,
     104,    1,  970,    2, 0x0a /* Public */,
     105,    0,  973,    2, 0x0a /* Public */,
     106,    0,  974,    2, 0x0a /* Public */,
     107,    0,  975,    2, 0x0a /* Public */,
     108,    0,  976,    2, 0x0a /* Public */,
     109,    0,  977,    2, 0x0a /* Public */,
     110,    0,  978,    2, 0x0a /* Public */,
     111,    0,  979,    2, 0x0a /* Public */,
     112,    0,  980,    2, 0x0a /* Public */,
     113,    1,  981,    2, 0x0a /* Public */,
     115,    1,  984,    2, 0x0a /* Public */,
     116,    2,  987,    2, 0x0a /* Public */,
     119,    1,  992,    2, 0x0a /* Public */,
     121,    1,  995,    2, 0x0a /* Public */,
     123,    3,  998,    2, 0x0a /* Public */,
     129,    0, 1005,    2, 0x0a /* Public */,
     130,    0, 1006,    2, 0x0a /* Public */,
     131,    0, 1007,    2, 0x0a /* Public */,
     132,    0, 1008,    2, 0x08 /* Private */,
     133,    0, 1009,    2, 0x08 /* Private */,
     134,    0, 1010,    2, 0x08 /* Private */,
     135,    0, 1011,    2, 0x08 /* Private */,
     136,    0, 1012,    2, 0x08 /* Private */,
     137,    0, 1013,    2, 0x08 /* Private */,
     138,    0, 1014,    2, 0x08 /* Private */,
     139,    0, 1015,    2, 0x08 /* Private */,
     140,    0, 1016,    2, 0x08 /* Private */,
     141,    0, 1017,    2, 0x08 /* Private */,
     142,    0, 1018,    2, 0x08 /* Private */,
     143,    0, 1019,    2, 0x08 /* Private */,
     144,    1, 1020,    2, 0x08 /* Private */,
     146,    1, 1023,    2, 0x08 /* Private */,
     148,    1, 1026,    2, 0x08 /* Private */,
     149,    1, 1029,    2, 0x08 /* Private */,
     150,    1, 1032,    2, 0x08 /* Private */,
     151,    0, 1035,    2, 0x08 /* Private */,
     152,    0, 1036,    2, 0x08 /* Private */,
     153,    1, 1037,    2, 0x08 /* Private */,
     154,    1, 1040,    2, 0x08 /* Private */,
     155,    1, 1043,    2, 0x08 /* Private */,
     156,    0, 1046,    2, 0x08 /* Private */,
     157,    0, 1047,    2, 0x08 /* Private */,
     158,    0, 1048,    2, 0x08 /* Private */,
     159,    0, 1049,    2, 0x08 /* Private */,
     160,    0, 1050,    2, 0x08 /* Private */,
     161,    1, 1051,    2, 0x08 /* Private */,
     162,    0, 1054,    2, 0x08 /* Private */,
     163,    0, 1055,    2, 0x08 /* Private */,
     164,    0, 1056,    2, 0x08 /* Private */,
     165,    0, 1057,    2, 0x08 /* Private */,
     166,    0, 1058,    2, 0x08 /* Private */,
     167,    0, 1059,    2, 0x08 /* Private */,
     168,    0, 1060,    2, 0x08 /* Private */,
     169,    1, 1061,    2, 0x08 /* Private */,
     170,    0, 1064,    2, 0x08 /* Private */,
     171,    1, 1065,    2, 0x08 /* Private */,
     172,    0, 1068,    2, 0x08 /* Private */,
     173,    0, 1069,    2, 0x08 /* Private */,
     174,    0, 1070,    2, 0x08 /* Private */,
     175,    0, 1071,    2, 0x08 /* Private */,
     176,    0, 1072,    2, 0x08 /* Private */,
     177,    1, 1073,    2, 0x08 /* Private */,
     178,    1, 1076,    2, 0x08 /* Private */,
     179,    1, 1079,    2, 0x08 /* Private */,
     180,    0, 1082,    2, 0x08 /* Private */,
     181,    0, 1083,    2, 0x08 /* Private */,
     182,    0, 1084,    2, 0x08 /* Private */,
     183,    1, 1085,    2, 0x08 /* Private */,
     184,    1, 1088,    2, 0x08 /* Private */,
     185,    0, 1091,    2, 0x08 /* Private */,
     186,    1, 1092,    2, 0x08 /* Private */,
     187,    1, 1095,    2, 0x08 /* Private */,
     188,    0, 1098,    2, 0x08 /* Private */,
     189,    1, 1099,    2, 0x08 /* Private */,
     190,    0, 1102,    2, 0x08 /* Private */,
     191,    0, 1103,    2, 0x08 /* Private */,
     192,    0, 1104,    2, 0x08 /* Private */,
     193,    0, 1105,    2, 0x08 /* Private */,
     194,    0, 1106,    2, 0x08 /* Private */,
     195,    1, 1107,    2, 0x08 /* Private */,
     196,    0, 1110,    2, 0x08 /* Private */,
     197,    1, 1111,    2, 0x08 /* Private */,
     198,    1, 1114,    2, 0x08 /* Private */,
     199,    1, 1117,    2, 0x08 /* Private */,
     200,    0, 1120,    2, 0x08 /* Private */,
     201,    0, 1121,    2, 0x08 /* Private */,
     202,    1, 1122,    2, 0x08 /* Private */,
     203,    0, 1125,    2, 0x08 /* Private */,
     204,    0, 1126,    2, 0x08 /* Private */,
     205,    0, 1127,    2, 0x08 /* Private */,
     206,    0, 1128,    2, 0x08 /* Private */,
     207,    0, 1129,    2, 0x08 /* Private */,
     208,    1, 1130,    2, 0x08 /* Private */,
     209,    1, 1133,    2, 0x08 /* Private */,
     210,    0, 1136,    2, 0x08 /* Private */,
     211,    1, 1137,    2, 0x08 /* Private */,
     212,    0, 1140,    2, 0x08 /* Private */,
     213,    0, 1141,    2, 0x08 /* Private */,
     214,    0, 1142,    2, 0x08 /* Private */,
     215,    0, 1143,    2, 0x08 /* Private */,
     216,    0, 1144,    2, 0x08 /* Private */,
     217,    0, 1145,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, QMetaType::Int, 0x80000000 | 9, QMetaType::Bool, QMetaType::Int,    4,    7,    8,   10,   11,   12,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int, 0x80000000 | 23,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   35,   36,
    QMetaType::Void, QMetaType::VoidStar, QMetaType::VoidStar, QMetaType::VoidStar,   38,   39,   40,
    QMetaType::Void,
    0x80000000 | 43,
    QMetaType::Void, 0x80000000 | 43,   45,
    0x80000000 | 47, 0x80000000 | 48, 0x80000000 | 50,   49,   51,
    QMetaType::Int, 0x80000000 | 53, 0x80000000 | 47,   54,   55,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::QString, QMetaType::QString, QMetaType::QString,   58,   59,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double,   73,   74,   75,   76,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Int,   80,   81,   82,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Int,   80,   81,   82,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Int,   80,   81,   82,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 88, 0x80000000 | 48, 0x80000000 | 47, 0x80000000 | 47,   89,   90,   91,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,   99,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,  101,  102,  103,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 114,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,  117,  118,
    QMetaType::Void, 0x80000000 | 120,   99,
    QMetaType::Void, 0x80000000 | 122,   99,
    QMetaType::Void, 0x80000000 | 124, QMetaType::Bool, 0x80000000 | 127,  125,  126,  128,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,  145,
    QMetaType::Void, QMetaType::Double,  147,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,  147,
    QMetaType::Void, QMetaType::Int,  147,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,  147,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,  147,
    QMetaType::Void, QMetaType::QString,  147,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,  147,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,  147,
    QMetaType::Void, QMetaType::Int,  147,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sig_updateBuffer((*reinterpret_cast< unsigned char*(*)>(_a[1]))); break;
        case 1: _t->imageProcessedFinish(); break;
        case 2: _t->startImageProcessing((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< std::string(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 3: _t->onCheckBox_Ori_2_changed(); break;
        case 4: _t->onCheckBox_Ori_changed(); break;
        case 5: _t->onPushButton_11_clicked(); break;
        case 6: _t->onTabIndexChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_insName_4_textChanged(); break;
        case 8: _t->process_buffer((*reinterpret_cast< unsigned char*(*)>(_a[1]))); break;
        case 9: _t->lineEdit_8returnPress(); break;
        case 10: _t->ReturnToCenter(); break;
        case 11: { int _r = _t->checkCheckedCheckBoxes((*reinterpret_cast< const QQueue<QCheckBox*>(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 12: _t->SetNormalGain(); break;
        case 13: _t->SetSpecularGain(); break;
        case 14: _t->SetDiffuseGain(); break;
        case 15: _t->SetShapeGain(); break;
        case 16: _t->DirectionChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->on_DirectionPoseBtn_clicked(); break;
        case 18: _t->LanChange(); break;
        case 19: _t->LanChange2(); break;
        case 20: _t->ABcheckboxConverse(); break;
        case 21: _t->DeviceInfoDisplay((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: _t->handleDeviceSelection((*reinterpret_cast< void*(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2])),(*reinterpret_cast< void*(*)>(_a[3]))); break;
        case 23: _t->slot_sendPercent(); break;
        case 24: { InsUpgrad _r = _t->ReadUpgradInfo();
            if (_a[0]) *reinterpret_cast< InsUpgrad*>(_a[0]) = std::move(_r); }  break;
        case 25: _t->WriteUpgradInfo((*reinterpret_cast< InsUpgrad(*)>(_a[1]))); break;
        case 26: { uint32_t _r = _t->ArrayToVariable32((*reinterpret_cast< uint8_t*(*)>(_a[1])),(*reinterpret_cast< uint8_t(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< uint32_t*>(_a[0]) = std::move(_r); }  break;
        case 27: { int _r = _t->FormatStrData((*reinterpret_cast< std::string*(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 28: _t->on_comboBox_7_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: { QString _r = _t->generateSystemModel((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 30: _t->ReadPose2(); break;
        case 31: _t->ABconverse2(); break;
        case 32: _t->ABencodePose(); break;
        case 33: _t->ABconverse(); break;
        case 34: _t->ReadPose(); break;
        case 35: _t->writeRegisters(); break;
        case 36: _t->ReadCamTem(); break;
        case 37: _t->ReadBoardTem(); break;
        case 38: _t->ReadFpgaTem(); break;
        case 39: _t->DevIp_clicked(); break;
        case 40: _t->updateImages(); break;
        case 41: _t->simulateUpdateBuffer(); break;
        case 42: _t->updateValues((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 43: _t->ImageValue_clicked(); break;
        case 44: _t->PoseBtn2_clicked(); break;
        case 45: _t->showXY((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 46: _t->showXY2((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 47: _t->showXY3((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 48: _t->PoseBtn_clicked(); break;
        case 49: _t->RegionBtn_clicked(); break;
        case 50: { Data_25D _r = _t->copyImageData((*reinterpret_cast< uint8_t*(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2])),(*reinterpret_cast< uint32_t(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< Data_25D*>(_a[0]) = std::move(_r); }  break;
        case 51: _t->on_FreLine_returnPressed(); break;
        case 52: _t->on_TriggerFre_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 53: _t->on_LightMode_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 54: _t->slot_btnOffsetMethod(); break;
        case 55: _t->on_LightBtn(); break;
        case 56: { int _r = _t->countCheckedCheckBoxes();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 57: _t->onCheckBoxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 58: _t->showImage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 59: _t->onComboBoxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 60: _t->clearImage(); break;
        case 61: _t->clearAllGraphicsViews(); break;
        case 62: _t->processAndDisplayImage(); break;
        case 63: _t->slot_Comm(); break;
        case 64: _t->slot_DeviceInfo(); break;
        case 65: _t->slot_LightEdit(); break;
        case 66: _t->slot_MainPage(); break;
        case 67: _t->InitWindow(); break;
        case 68: _t->update_buffer((*reinterpret_cast< InsPMDL_Buffer*(*)>(_a[1]))); break;
        case 69: _t->slot_updateBuffer((*reinterpret_cast< unsigned char*(*)>(_a[1]))); break;
        case 70: _t->addToStateMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 71: _t->modbus_state_change_slot((*reinterpret_cast< QModbusDevice::State(*)>(_a[1]))); break;
        case 72: _t->modbus_error_occurred_slot((*reinterpret_cast< QModbusDevice::Error(*)>(_a[1]))); break;
        case 73: _t->modbus_read_over_slot((*reinterpret_cast< QModbusDataUnit::RegisterType(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< QVector<quint16>(*)>(_a[3]))); break;
        case 74: _t->modbusPeriodRead(); break;
        case 75: _t->readReady(); break;
        case 76: _t->slot_infoUpdate(); break;
        case 77: _t->on_StateSaveBtn_clicked(); break;
        case 78: _t->on_StateClearBtn_clicked(); break;
        case 79: _t->on_toolButton_clicked(); break;
        case 80: _t->on_toolButton_2_clicked(); break;
        case 81: _t->on_pushButton_clicked(); break;
        case 82: _t->on_pushButton_2_clicked(); break;
        case 83: _t->on_toolButton_3_clicked(); break;
        case 84: _t->on_toolButton_4_clicked(); break;
        case 85: _t->on_connectBtn_clicked(); break;
        case 86: _t->on_startAcqBtn_clicked(); break;
        case 87: _t->on_stopAcqBtn_clicked(); break;
        case 88: _t->on_disConnectBtn_clicked(); break;
        case 89: _t->on_checkBox_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 90: _t->on_doubleSpinBox_3_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 91: _t->on_ChuFaCombox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 92: _t->on_BeiPinSpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 93: _t->on_FenPinSpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 94: _t->on_ModbusClientBtn_10_clicked(); break;
        case 95: _t->on_ClearBtn_2_clicked(); break;
        case 96: _t->on_EnCodeTriComBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 97: _t->on_LightMode_2_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 98: _t->on_comboBox_3_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 99: _t->on_ModbusClientBtn_11_clicked(); break;
        case 100: _t->on_ClearBtn_3_clicked(); break;
        case 101: _t->on_insInterface_5_returnPressed(); break;
        case 102: _t->on_insInterface_8_returnPressed(); break;
        case 103: _t->on_insInterface_6_returnPressed(); break;
        case 104: _t->on_comboBox_4_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 105: _t->on_pushButton_5_clicked(); break;
        case 106: _t->on_pushButton_6_clicked(); break;
        case 107: _t->on_pushButton_7_clicked(); break;
        case 108: _t->on_pushButton_3_clicked(); break;
        case 109: _t->on_lineEdit_9_returnPressed(); break;
        case 110: _t->on_lineEdit_10_returnPressed(); break;
        case 111: _t->on_lineEdit_7_returnPressed(); break;
        case 112: _t->on_insName_10_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 113: _t->on_ExNumUI_returnPressed(); break;
        case 114: _t->on_ChuFaCombox_2_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 115: _t->on_ExDelayUI_returnPressed(); break;
        case 116: _t->on_ExWidthUI_returnPressed(); break;
        case 117: _t->on_CamTriUI_returnPressed(); break;
        case 118: _t->on_lineEdit_8_returnPressed(); break;
        case 119: _t->on_lineEdit_11_returnPressed(); break;
        case 120: _t->on_LightMode_5_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 121: _t->on_LightMode_4_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 122: _t->on_LightMode_6_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 123: _t->loadConfigure(); break;
        case 124: _t->saveConfigure(); break;
        case 125: _t->saveConfigureFlash(); break;
        case 126: _t->on_checkBox_SP2_2_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 127: _t->on_insInterface_4_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 128: _t->on_lineEdit_12_returnPressed(); break;
        case 129: _t->on_LightMode3_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 130: _t->on_checkBox_SP2_3_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 131: _t->on_lineEdit_18_returnPressed(); break;
        case 132: _t->on_LightMode_3_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 133: _t->on_lineEdit_13_returnPressed(); break;
        case 134: _t->on_lineEdit_16_returnPressed(); break;
        case 135: _t->on_lineEdit_15_returnPressed(); break;
        case 136: _t->on_lineEdit_17_returnPressed(); break;
        case 137: _t->on_lineEdit_14_returnPressed(); break;
        case 138: _t->on_LightMode_7_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 139: _t->on_lineEdit_23_returnPressed(); break;
        case 140: _t->on_ChuFaCombox_3_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 141: _t->on_FenPinSpinBox_2_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 142: _t->on_BeiPinSpinBox_2_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 143: _t->on_lineEdit_24_returnPressed(); break;
        case 144: _t->on_lineEdit_25_returnPressed(); break;
        case 145: _t->on_LightMode_8_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 146: _t->on_lineEdit_26_returnPressed(); break;
        case 147: _t->on_lineEdit_27_returnPressed(); break;
        case 148: _t->on_lineEdit_28_returnPressed(); break;
        case 149: _t->on_lineEdit_29_returnPressed(); break;
        case 150: _t->on_lineEdit_30_returnPressed(); break;
        case 151: _t->on_LightMode_14_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 152: _t->on_LightMode_12_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 153: _t->on_pushButton_4_clicked(); break;
        case 154: _t->on_comboBox_6_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 155: _t->on_lineEdit_19_returnPressed(); break;
        case 156: _t->on_lineEdit_20_returnPressed(); break;
        case 157: _t->on_lineEdit_21_returnPressed(); break;
        case 158: _t->on_pushButton_8_clicked(); break;
        case 159: _t->on_pushButton_9_clicked(); break;
        case 160: _t->on_pushButton_10_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< unsigned char* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< unsigned char* >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< unsigned char* >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QQueue<QCheckBox*> >(); break;
            }
            break;
        case 69:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< unsigned char* >(); break;
            }
            break;
        case 71:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QModbusDevice::State >(); break;
            }
            break;
        case 72:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QModbusDevice::Error >(); break;
            }
            break;
        case 73:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QModbusDataUnit::RegisterType >(); break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<quint16> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(unsigned char * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::sig_updateBuffer)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::imageProcessedFinish)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(unsigned char * , int , int , std::string , bool , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::startImageProcessing)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 161)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 161;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 161)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 161;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::sig_updateBuffer(unsigned char * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::imageProcessedFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MainWindow::startImageProcessing(unsigned char * _t1, int _t2, int _t3, std::string _t4, bool _t5, int _t6)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
