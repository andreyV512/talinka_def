object SolidGroupOptionsForm: TSolidGroupOptionsForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1075#1088#1091#1087#1087#1099' '#1087#1088#1086#1095#1085#1086#1089#1090#1080
  ClientHeight = 169
  ClientWidth = 251
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 56
    Width = 167
    Height = 13
    Caption = #1043#1088#1091#1087#1087#1072' '#1087#1088#1086#1095#1085#1086#1089#1090#1080' '#1087#1086' '#1091#1084#1086#1083#1095#1072#1085#1080#1102
  end
  object cbSolidGroupSwitch: TCheckBox
    Left = 8
    Top = 24
    Width = 225
    Height = 17
    Caption = #1048#1089#1087#1086#1083#1100#1079#1086#1074#1072#1090#1100' '#1084#1086#1076#1091#1083#1100' '#1075#1088#1091#1087#1087#1099' '#1087#1088#1086#1095#1085#1086#1089#1090#1080
    TabOrder = 0
    OnClick = cbSolidGroupSwitchClick
  end
  object cboxSelectGroup: TComboBox
    Left = 8
    Top = 75
    Width = 73
    Height = 21
    TabOrder = 1
    Text = 'K'
    OnChange = cboxSelectGroupChange
    Items.Strings = (
      'K'
      'E'
      'L'
      'M'
      'P')
  end
end
