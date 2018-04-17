object DefectoscopersForm: TDefectoscopersForm
  Left = 0
  Top = 0
  Caption = #1044#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1089#1090#1099
  ClientHeight = 330
  ClientWidth = 435
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
  object DBGridDefectoscopers: TDBGrid
    Left = 0
    Top = 41
    Width = 435
    Height = 200
    Align = alTop
    DataSource = DataDefectoscopers
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
        FieldName = 'Name'
        Title.Alignment = taCenter
        Title.Caption = #1048#1084#1103' '#1076#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1089#1090#1072
        Width = 339
        Visible = True
      end>
  end
  object Panel: TPanel
    Left = 0
    Top = 0
    Width = 435
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
      Left = 150
      Top = 9
      Width = 135
      Height = 24
      Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1086#1087#1077#1088#1072#1090#1086#1088#1072
      TabOrder = 0
      OnClick = bAddClick
    end
    object bRefresh: TButton
      Left = 10
      Top = 9
      Width = 135
      Height = 24
      Caption = #1054#1073#1085#1086#1074#1080#1090#1100' '#1089#1087#1080#1089#1086#1082
      TabOrder = 1
      OnClick = bRefreshClick
    end
    object bPasswordChange: TButton
      Left = 290
      Top = 9
      Width = 135
      Height = 24
      Caption = #1057#1084#1077#1085#1072' '#1087#1072#1088#1086#1083#1103
      TabOrder = 2
      OnClick = bPasswordChangeClick
    end
  end
  object pmExport: TPopupMenu
    Left = 41
    Top = 277
    object SelectAll: TMenuItem
      Caption = #1042#1099#1076#1077#1083#1080#1090#1100' '#1074#1089#1105
      OnClick = SelectAllClick
    end
    object DeleteEtalonCheck: TMenuItem
      Caption = #1059#1076#1072#1083#1080#1090#1100
      OnClick = DeleteDefectoscoper
    end
  end
  object QDefectoscopers: TADOQuery
    Parameters = <>
    SQL.Strings = (
      'Select * from dbo.EtalonDefect')
    Left = 124
    Top = 277
  end
  object DataDefectoscopers: TDataSource
    DataSet = QDefectoscopers
    Left = 222
    Top = 278
  end
end
