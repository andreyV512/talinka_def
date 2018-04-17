object EtalonTubeDefectsNewForm: TEtalonTubeDefectsNewForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize, biMaximize, biHelp]
  Caption = #1050#1086#1085#1090#1088#1086#1083#1100#1085#1099#1077' '#1090#1086#1095#1082#1080' '#1085#1086#1074#1086#1075#1086' '#1057#1054#1055
  ClientHeight = 428
  ClientWidth = 828
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnKeyPress = FormKeyPress
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object DBGridEtalons: TDBGrid
    Left = 0
    Top = 41
    Width = 828
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
        Expanded = False
        FieldName = 'IDTube'
        Title.Alignment = taCenter
        Title.Caption = 'ID '#1069#1090#1072#1083#1086#1085#1072
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'Name'
        Title.Alignment = taCenter
        Title.Caption = #1053#1072#1079#1074#1072#1085#1080#1077
        Width = 73
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
        FieldName = 'Module'
        Title.Alignment = taCenter
        Title.Caption = #1052#1086#1076#1091#1083#1100
        Width = 75
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'DefType'
        Title.Alignment = taCenter
        Title.Caption = #1058#1080#1087' '#1076#1077#1092#1077#1082#1090#1072
        Width = 84
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'DefPoint'
        Title.Alignment = taCenter
        Title.Caption = #1058#1086#1095#1082#1072' '#1082#1086#1085#1090#1088#1086#1083#1103
        Width = 88
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'DefRadius'
        Title.Alignment = taCenter
        Title.Caption = #1056#1072#1076#1080#1091#1089' '#1087#1086#1080#1089#1082#1072
        Width = 85
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'isTopBrackBorder'
        Title.Alignment = taCenter
        Title.Caption = #1042#1077#1088#1093' '#1055#1086#1088#1086#1075
        Width = 61
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'TopBorderPercent'
        Title.Alignment = taCenter
        Title.Caption = #1042#1077#1088#1093' %'
        Width = 51
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'isBottomBrackBorder'
        Title.Alignment = taCenter
        Title.Caption = #1053#1080#1079' '#1055#1086#1088#1086#1075
        Width = 54
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'BottomBorderPercent'
        Title.Alignment = taCenter
        Title.Caption = #1053#1080#1079' %'
        Width = 44
        Visible = True
      end>
  end
  object Panel: TPanel
    Left = 0
    Top = 0
    Width = 828
    Height = 41
    Align = alTop
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object bAdd: TButton
      Left = 14
      Top = 10
      Width = 100
      Height = 25
      Caption = #1044#1086#1073#1072#1074#1080#1090#1100
      TabOrder = 0
      OnClick = bAddClick
    end
    object bAbort: TButton
      Left = 340
      Top = 10
      Width = 100
      Height = 25
      Caption = #1054#1090#1084#1077#1085#1072
      TabOrder = 3
      OnClick = bAbortClick
    end
    object bSave: TButton
      Left = 450
      Top = 10
      Width = 100
      Height = 25
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
      TabOrder = 4
      OnClick = bSaveClick
    end
    object bCopy: TButton
      Left = 230
      Top = 10
      Width = 100
      Height = 25
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100
      TabOrder = 2
      OnClick = bCopyClick
    end
    object bEdit: TButton
      Left = 120
      Top = 10
      Width = 100
      Height = 25
      Caption = #1056#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1090#1100
      TabOrder = 1
      OnClick = bEditClick
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
    Left = 156
    Top = 284
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
    Left = 269
    Top = 284
  end
  object QEtalons: TADOQuery
    Parameters = <>
    SQL.Strings = (
      'Select * from dbo.EtalonTubeDefects')
    Left = 204
    Top = 284
  end
end
