object NewDefectoscoperForm: TNewDefectoscoperForm
  Left = 0
  Top = 0
  Caption = #1053#1086#1074#1099#1081' '#1076#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1089#1090
  ClientHeight = 149
  ClientWidth = 264
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnClose = FormClose
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lName: TLabel
    Left = 16
    Top = 13
    Width = 113
    Height = 13
    Caption = #1048#1084#1103' '#1076#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1089#1090#1072
  end
  object lPassword: TLabel
    Left = 16
    Top = 66
    Width = 37
    Height = 13
    Caption = #1055#1072#1088#1086#1083#1100
  end
  object eName: TEdit
    Left = 16
    Top = 37
    Width = 225
    Height = 21
    TabOrder = 0
  end
  object ePassword: TEdit
    Left = 16
    Top = 87
    Width = 225
    Height = 21
    TabOrder = 1
  end
  object bAdd: TButton
    Left = 40
    Top = 116
    Width = 169
    Height = 25
    Caption = #1044#1086#1073#1072#1074#1080#1090#1100
    TabOrder = 2
    OnClick = bAddClick
  end
  object QDefectoscoper: TADOQuery
    Parameters = <>
    SQL.Strings = (
      'Select * from dbo.EtalonDefect')
    Left = 228
    Top = 5
  end
end
