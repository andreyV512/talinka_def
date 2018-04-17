object EtalonDefectForm: TEtalonDefectForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize, biMaximize, biHelp]
  Caption = #1057#1087#1080#1089#1086#1082' '#1057#1054#1055
  ClientHeight = 352
  ClientWidth = 337
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
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object DBGridEtalons: TDBGrid
    Left = 0
    Top = 41
    Width = 337
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
        Expanded = False
        FieldName = 'ID'
        Title.Alignment = taCenter
        Width = 50
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
        Expanded = False
        FieldName = 'Date'
        Title.Alignment = taCenter
        Title.Caption = #1044#1072#1090#1072' '#1079#1072#1085#1077#1089#1077#1085#1080#1103
        Width = 119
        Visible = True
      end>
  end
  object Panel: TPanel
    Left = 0
    Top = 0
    Width = 337
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
    object cbTypeSize: TComboBox
      Left = 88
      Top = 9
      Width = 57
      Height = 24
      Style = csDropDownList
      TabOrder = 0
    end
    object View: TButton
      Left = 152
      Top = 8
      Width = 75
      Height = 25
      Caption = #1055#1086#1082#1072#1079#1072#1090#1100
      TabOrder = 1
      OnClick = ViewClick
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
  object bCheck: TButton
    Left = 8
    Top = 278
    Width = 119
    Height = 42
    Caption = #1057#1074#1077#1088#1080#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = bCheckClick
  end
  object pmExport: TPopupMenu
    Left = 154
    Top = 285
    object ViewEtalon: TMenuItem
      Caption = #1055#1086#1082#1072#1079#1072#1090#1100' '#1082#1086#1085#1090#1088#1086#1083#1100#1085#1099#1077' '#1090#1086#1095#1082#1080
      OnClick = ViewEtalonTubeDefects
    end
    object SelectAll: TMenuItem
      Caption = #1042#1099#1076#1077#1083#1080#1090#1100' '#1074#1089#1105
      OnClick = SelectAllClick
    end
    object DeleteEtalon: TMenuItem
      Caption = #1059#1076#1072#1083#1080#1090#1100
      OnClick = DeleteEtalonDefect
    end
  end
  object DataEtalons: TDataSource
    DataSet = QEtalons
    Left = 261
    Top = 284
  end
  object QEtalons: TADOQuery
    Parameters = <>
    SQL.Strings = (
      'Select * from dbo.EtalonDefect')
    Left = 212
    Top = 285
  end
end
