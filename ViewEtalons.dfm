object FormViewEtalons: TFormViewEtalons
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize, biMaximize, biHelp]
  Caption = #1055#1088#1086#1089#1084#1086#1090#1088' '#1101#1090#1072#1083#1086#1085#1086#1074
  ClientHeight = 428
  ClientWidth = 708
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
  PixelsPerInch = 96
  TextHeight = 13
  object DBGridEtalons: TDBGrid
    Left = 0
    Top = 41
    Width = 708
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
        Width = 50
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'Name'
        Title.Caption = #1043#1055
        Width = 40
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
        Title.Caption = #1044#1072#1090#1072' '#1079#1072#1085#1077#1089#1077#1085#1080#1103
        Width = 119
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'p1'
        Title.Alignment = taCenter
        Width = 50
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'p2'
        Title.Alignment = taCenter
        Width = 50
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'p3'
        Title.Alignment = taCenter
        Width = 50
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'p4'
        Title.Alignment = taCenter
        Width = 50
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'p5'
        Title.Alignment = taCenter
        Width = 50
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'p6'
        Title.Alignment = taCenter
        Width = 50
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'p7'
        Title.Alignment = taCenter
        Width = 50
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'p8'
        Title.Alignment = taCenter
        Width = 50
        Visible = True
      end
      item
        Alignment = taCenter
        Expanded = False
        FieldName = 'Si'
        Title.Alignment = taCenter
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Mn'
        Title.Alignment = taCenter
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Cr'
        Title.Alignment = taCenter
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Ni'
        Title.Alignment = taCenter
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Mo'
        Title.Alignment = taCenter
        Width = 50
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Si_elem'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Si_id'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Mn_elem'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Mn_id'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Cr_elem'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Cr_id'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Ni_elem'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Ni_id'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Mo_elem'
        Visible = True
      end
      item
        Expanded = False
        FieldName = 'Mo_id'
        Visible = True
      end>
  end
  object Panel: TPanel
    Left = 0
    Top = 0
    Width = 708
    Height = 41
    Align = alTop
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    ExplicitWidth = 854
    object LSG: TLabel
      Left = 19
      Top = 13
      Width = 106
      Height = 16
      Caption = #1043#1088#1091#1087#1087#1072' '#1087#1088#1086#1095#1085#1086#1089#1090#1080
    end
    object LTS: TLabel
      Left = 203
      Top = 13
      Width = 71
      Height = 16
      Caption = #1058#1080#1087#1086#1088#1072#1079#1084#1077#1088
    end
    object ComboBoxSG: TComboBox
      Left = 131
      Top = 9
      Width = 57
      Height = 24
      Style = csDropDownList
      TabOrder = 0
    end
    object ComboBoxTS: TComboBox
      Left = 280
      Top = 9
      Width = 57
      Height = 24
      Style = csDropDownList
      TabOrder = 1
    end
    object View: TButton
      Left = 346
      Top = 8
      Width = 75
      Height = 25
      Caption = #1055#1086#1082#1072#1079#1072#1090#1100
      TabOrder = 2
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
  object pmExport: TPopupMenu
    Left = 436
    Top = 252
    object N1: TMenuItem
      Caption = #1055#1086#1089#1090#1088#1086#1080#1090#1100' '#1075#1088#1072#1092#1080#1082#1080
      OnClick = N1Click
    end
    object SelectAll: TMenuItem
      Caption = #1042#1099#1076#1077#1083#1080#1090#1100' '#1074#1089#1105
      OnClick = SelectAllClick
    end
    object N2: TMenuItem
      Caption = #1059#1076#1072#1083#1080#1090#1100
      OnClick = N2Click
    end
  end
  object SaveDialog: TSaveDialog
    DefaultExt = '.txt'
    Filter = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1043#1055' (*.txt)|*.txt'
    FilterIndex = 0
    Options = [ofHideReadOnly, ofNoChangeDir, ofEnableSizing]
    Left = 320
    Top = 252
  end
  object OpenDialog: TOpenDialog
    Options = [ofHideReadOnly, ofNoChangeDir, ofEnableSizing]
    Left = 380
    Top = 252
  end
  object DataEtalons: TDataSource
    DataSet = QEtalons
    Left = 544
    Top = 252
  end
  object QEtalons: TADOQuery
    Parameters = <>
    SQL.Strings = (
      'Select * from dbo.SGEtalon')
    Left = 492
    Top = 252
  end
end
