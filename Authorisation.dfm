object AuthorisationForm: TAuthorisationForm
  Left = 0
  Top = 0
  Caption = #1040#1074#1090#1086#1088#1080#1079#1072#1094#1080#1103
  ClientHeight = 116
  ClientWidth = 259
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnClose = FormClose
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
  object cbSelectDefectoscoper: TComboBox
    Left = 16
    Top = 30
    Width = 225
    Height = 27
    Style = csDropDownList
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
  end
  object bChoose: TButton
    Left = 40
    Top = 71
    Width = 169
    Height = 25
    Caption = #1042#1099#1073#1088#1072#1090#1100
    TabOrder = 1
    OnClick = bChooseClick
  end
  object QDefectoscoper: TADOQuery
    Parameters = <>
    SQL.Strings = (
      'Select * from dbo.EtalonDefect')
    Left = 216
    Top = 61
  end
end
