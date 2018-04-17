object EtalonNewDefectForm: TEtalonNewDefectForm
  Left = 0
  Top = 0
  Caption = #1050#1086#1085#1090#1088#1086#1083#1100#1085#1072#1103' '#1090#1086#1095#1082#1072
  ClientHeight = 290
  ClientWidth = 567
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
  object bAbort: TButton
    Left = 301
    Top = 242
    Width = 137
    Height = 35
    Caption = #1054#1090#1084#1077#1085#1072
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = bAbortClick
  end
  object gbTube: TGroupBox
    Left = 8
    Top = 8
    Width = 258
    Height = 164
    Caption = #1057#1054#1055
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object lIDTube: TLabel
      Left = 16
      Top = 18
      Width = 83
      Height = 19
      Caption = 'ID '#1069#1090#1072#1083#1086#1085#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lName: TLabel
      Left = 16
      Top = 63
      Width = 68
      Height = 19
      Caption = #1053#1072#1079#1074#1072#1085#1080#1077
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lTypeSize: TLabel
      Left = 16
      Top = 108
      Width = 88
      Height = 19
      Caption = #1058#1080#1087#1086#1088#1072#1079#1084#1077#1088
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object eTypeSize: TEdit
      Left = 123
      Top = 105
      Width = 121
      Height = 27
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
    end
    object eName: TEdit
      Left = 123
      Top = 60
      Width = 121
      Height = 27
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
    end
    object eIDTube: TEdit
      Left = 123
      Top = 15
      Width = 121
      Height = 27
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
  end
  object gbDefect: TGroupBox
    Left = 275
    Top = 8
    Width = 282
    Height = 228
    Caption = #1050#1086#1085#1090#1088#1086#1083#1100#1085#1072#1103' '#1090#1086#1095#1082#1072
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object lTopBorder: TLabel
      Left = 15
      Top = 158
      Width = 124
      Height = 19
      Caption = #1042#1077#1088#1093#1085#1103#1103' '#1043#1088#1072#1085#1080#1094#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lDefPoint: TLabel
      Left = 15
      Top = 88
      Width = 118
      Height = 19
      Caption = #1058#1086#1095#1082#1072' '#1082#1086#1085#1090#1088#1086#1083#1103
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lDefType: TLabel
      Left = 15
      Top = 53
      Width = 96
      Height = 19
      Caption = #1058#1080#1087' '#1044#1077#1092#1077#1082#1090#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lDefRadius: TLabel
      Left = 15
      Top = 123
      Width = 105
      Height = 19
      Caption = #1056#1072#1076#1080#1091#1089' '#1087#1086#1080#1089#1082#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lModule: TLabel
      Left = 15
      Top = 18
      Width = 55
      Height = 19
      Caption = #1052#1086#1076#1091#1083#1100
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lTopPercent: TLabel
      Left = 253
      Top = 158
      Width = 16
      Height = 19
      Caption = '%'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lBottomPercent: TLabel
      Left = 253
      Top = 193
      Width = 16
      Height = 19
      Caption = '%'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lBottomBorder: TLabel
      Left = 15
      Top = 193
      Width = 122
      Height = 19
      Caption = #1053#1080#1078#1085#1103#1103' '#1043#1088#1072#1085#1080#1094#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object eDefPoint: TEdit
      Left = 148
      Top = 85
      Width = 121
      Height = 27
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
    end
    object eDefRadius: TEdit
      Left = 148
      Top = 120
      Width = 121
      Height = 27
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
    end
    object eTopPercentage: TEdit
      Left = 220
      Top = 155
      Width = 33
      Height = 27
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      Text = '0'
    end
    object eBottomPercentage: TEdit
      Left = 220
      Top = 190
      Width = 33
      Height = 27
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 7
      Text = '0'
    end
    object cbModule: TComboBox
      Left = 148
      Top = 15
      Width = 121
      Height = 27
      Style = csDropDownList
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemIndex = 0
      ParentFont = False
      TabOrder = 0
      Text = #1055#1086#1087#1077#1088#1077#1095#1085#1099#1081
      OnChange = cbModuleChange
      Items.Strings = (
        #1055#1086#1087#1077#1088#1077#1095#1085#1099#1081
        #1055#1088#1086#1076#1086#1083#1100#1085#1099#1081
        #1058#1086#1083#1097#1080#1085#1086#1084#1077#1088)
    end
    object cbTopBorder: TComboBox
      Left = 148
      Top = 155
      Width = 73
      Height = 27
      Style = csDropDownList
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemIndex = 1
      ParentFont = False
      TabOrder = 4
      Text = #1041#1088#1072#1082
      Items.Strings = (
        '2 '#1050#1083#1072#1089#1089
        #1041#1088#1072#1082)
    end
    object cbBottomBorder: TComboBox
      Left = 148
      Top = 190
      Width = 73
      Height = 27
      Style = csDropDownList
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemIndex = 1
      ParentFont = False
      TabOrder = 6
      Text = #1041#1088#1072#1082
      Items.Strings = (
        '2 '#1050#1083#1072#1089#1089
        #1041#1088#1072#1082)
    end
    object cbDefType: TComboBox
      Left = 148
      Top = 50
      Width = 121
      Height = 27
      Style = csDropDownList
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemIndex = 0
      ParentFont = False
      TabOrder = 1
      Text = #1044#1086' 3 '#1089#1086#1089#1077#1076#1085#1080#1093
      Items.Strings = (
        #1044#1086' 3 '#1089#1086#1089#1077#1076#1085#1080#1093
        #1042#1089#1077' '#1076#1072#1090#1095#1080#1082#1080)
    end
  end
  object bSave: TButton
    Left = 134
    Top = 242
    Width = 137
    Height = 35
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = bSaveClick
  end
  object bManualBorderSet: TButton
    Left = 40
    Top = 178
    Width = 226
    Height = 41
    Caption = #1056#1091#1095#1085#1072#1103' '#1085#1072#1089#1090#1088#1086#1081#1082#1072' '#1087#1086#1088#1086#1075#1086#1074
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    WordWrap = True
    OnClick = bManualBorderSetClick
  end
  object QEtalons: TADOQuery
    Parameters = <>
    SQL.Strings = (
      'Select * from dbo.EtalonDefect')
    Left = 37
    Top = 237
  end
end
