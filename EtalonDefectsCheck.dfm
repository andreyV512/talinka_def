object EtalonDefectsCheckForm: TEtalonDefectsCheckForm
  Left = 0
  Top = 0
  Caption = #1057#1074#1077#1088#1082#1072' '#1089' '#1057#1054#1055
  ClientHeight = 235
  ClientWidth = 563
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lDefectoscoper: TLabel
    Left = 26
    Top = 56
    Width = 86
    Height = 13
    Caption = #1044#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1089#1090
  end
  object lDate: TLabel
    Left = 231
    Top = 56
    Width = 68
    Height = 13
    Caption = #1044#1072#1090#1072' '#1080' '#1042#1088#1077#1084#1103
  end
  object lEtalonName: TLabel
    Left = 26
    Top = 112
    Width = 48
    Height = 13
    Caption = #1053#1072#1079#1074#1072#1085#1080#1077
  end
  object lTypeSize: TLabel
    Left = 231
    Top = 112
    Width = 59
    Height = 13
    Caption = #1058#1080#1087#1086#1088#1072#1079#1084#1077#1088
  end
  object pResultNum: TPanel
    Left = 21
    Top = 16
    Width = 171
    Height = 33
    BorderStyle = bsSingle
    Caption = #1063#1080#1089#1083#1086' '#1089#1086#1074#1087#1072#1074#1096#1080#1093
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 6
  end
  object pResultBit: TPanel
    Left = 223
    Top = 16
    Width = 171
    Height = 33
    BorderStyle = bsSingle
    Caption = #1056#1077#1079#1091#1083#1100#1090#1072#1090
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentFont = False
    TabOrder = 7
  end
  object eDate: TEdit
    Left = 223
    Top = 73
    Width = 171
    Height = 27
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 1
  end
  object eTypeSize: TEdit
    Left = 223
    Top = 129
    Width = 171
    Height = 27
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 3
  end
  object eEtalonName: TEdit
    Left = 21
    Top = 129
    Width = 171
    Height = 27
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 2
  end
  object bSave: TButton
    Left = 21
    Top = 177
    Width = 171
    Height = 40
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = bSaveClick
  end
  object bCancel: TButton
    Left = 223
    Top = 177
    Width = 171
    Height = 40
    Caption = #1054#1090#1084#1077#1085#1072
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = bCancelClick
  end
  object mCheckedDefects: TMemo
    Left = 410
    Top = 16
    Width = 145
    Height = 201
    Lines.Strings = (
      'mCheckedDefects')
    ScrollBars = ssVertical
    TabOrder = 8
  end
  object cbSelectDefectoscoper: TComboBox
    Left = 21
    Top = 73
    Width = 171
    Height = 27
    Style = csDropDownList
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnChange = cbSelectDefectoscoperChange
  end
  object QEtalons: TADOQuery
    Parameters = <>
    SQL.Strings = (
      'Select * from dbo.EtalonDefect')
    Left = 388
    Top = 38
  end
  object SaveToFileDialog: TSaveDialog
    DefaultExt = '.dkb'
    Filter = #1044#1072#1085#1085#1099#1077' '#1087#1086' '#1090#1088#1091#1073#1077'(.dkb)|*.dkb|'#1042#1089#1077' '#1092#1072#1081#1083#1099'|*.*'
    Options = [ofHideReadOnly, ofNoChangeDir, ofShowHelp, ofEnableSizing]
    Left = 384
    Top = 88
  end
end
