object EtalonCheckJournalForm: TEtalonCheckJournalForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize, biMaximize, biHelp]
  Caption = #1046#1091#1088#1085#1072#1083' '#1082#1086#1085#1090#1088#1086#1083#1103' '#1057#1054#1055
  ClientHeight = 352
  ClientWidth = 740
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnKeyPress = FormKeyPress
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object DBGridEtalons: TDBGrid
    Left = 0
    Top = 41
    Width = 740
    Height = 200
    Align = alTop
    DataSource = DataEtalons
    Options = [dgTitles, dgIndicator, dgColumnResize, dgColLines, dgRowLines, dgTabs, dgRowSelect, dgConfirmDelete, dgCancelOnExit, dgMultiSelect, dgTitleClick, dgTitleHotTrack]
    PopupMenu = pmExport
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
    Columns = <
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'ID'
        Title.Alignment = taCenter
        Width = 50
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'Date'
        Title.Alignment = taCenter
        Title.Caption = #1044#1072#1090#1072' '#1079#1072#1085#1077#1089#1077#1085#1080#1103
        Width = 119
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'Name'
        Title.Alignment = taCenter
        Title.Caption = #1053#1072#1079#1074#1072#1085#1080#1077
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'TypeSize'
        Title.Alignment = taCenter
        Title.Caption = #1058#1080#1087#1086#1088#1072#1079#1084#1077#1088
        Width = 70
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'Result'
        Title.Alignment = taCenter
        Title.Caption = #1056#1077#1079#1091#1083#1100#1090#1072#1090
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'MatchedDefects'
        Title.Alignment = taCenter
        Title.Caption = #1057#1086#1074#1087#1072#1076#1077#1085#1080#1081
        Width = 74
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'NumOfDefects'
        Title.Alignment = taCenter
        Title.Caption = #1044#1077#1092#1077#1082#1090#1086#1074
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'Defectoscoper'
        Title.Alignment = taCenter
        Title.Caption = #1044#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1089#1090
        Width = 104
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'SaveAddress'
        Title.Alignment = taCenter
        Title.Caption = #1040#1076#1088#1077#1089' '#1092#1072#1081#1083#1072
        Width = 395
        Visible = True
      end>
  end
  object Panel: TPanel
    Left = 0
    Top = 0
    Width = 740
    Height = 41
    Align = alTop
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object LTS: TLabel
      Left = 9
      Top = 13
      Width = 71
      Height = 16
      Caption = #1058#1080#1087#1086#1088#1072#1079#1084#1077#1088
    end
    object lDateFrom: TLabel
      Left = 182
      Top = 13
      Width = 39
      Height = 16
      Caption = #1044#1072#1090#1072' '#1089
    end
    object lDateTo: TLabel
      Left = 364
      Top = 12
      Width = 14
      Height = 16
      Caption = #1087#1086
    end
    object cbTypeSize: TComboBox
      Left = 88
      Top = 9
      Width = 57
      Height = 24
      Style = csDropDownList
      TabOrder = 0
    end
    object View: TButton
      Left = 514
      Top = 9
      Width = 75
      Height = 24
      Caption = #1055#1086#1082#1072#1079#1072#1090#1100
      TabOrder = 3
      OnClick = ViewClick
    end
    object dtpTo: TDateTimePicker
      Left = 385
      Top = 9
      Width = 113
      Height = 24
      Date = 41826.341551643520000000
      Time = 41826.341551643520000000
      TabOrder = 2
    end
    object dtpFrom: TDateTimePicker
      Left = 229
      Top = 9
      Width = 113
      Height = 24
      Date = 41826.341551643520000000
      Time = 41826.341551643520000000
      TabOrder = 1
    end
  end
  object DBNavigatorEtalons: TDBNavigator
    Left = 0
    Top = 247
    Width = 212
    Height = 25
    DataSource = DataEtalons
    VisibleButtons = [nbFirst, nbPrior, nbNext, nbLast]
    TabOrder = 2
  end
  object pmExport: TPopupMenu
    Left = 154
    Top = 285
    object LoadTube: TMenuItem
      Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100' '#1090#1088#1091#1073#1091
      Visible = False
      OnClick = LoadTubeClick
    end
    object SelectAll: TMenuItem
      Caption = #1042#1099#1076#1077#1083#1080#1090#1100' '#1074#1089#1105
      OnClick = SelectAllClick
    end
    object DeleteEtalonCheck: TMenuItem
      Caption = #1059#1076#1072#1083#1080#1090#1100
      OnClick = DeleteEtalonCheckResult
    end
  end
  object DataEtalons: TDataSource
    DataSet = QChecks
    Left = 269
    Top = 284
  end
  object QChecks: TADOQuery
    Parameters = <>
    SQL.Strings = (
      'Select * from dbo.EtalonDefect')
    Left = 212
    Top = 285
  end
end
