object ADCSettForm: TADCSettForm
  Left = 177
  Top = 186
  BorderStyle = bsSingle
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1082#1086#1085#1090#1088#1086#1083#1103
  ClientHeight = 716
  ClientWidth = 996
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnActivate = FormActivate
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl: TPageControl
    Left = 0
    Top = 0
    Width = 996
    Height = 646
    ActivePage = ppPage
    Align = alClient
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object ppPage: TTabSheet
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1082#1086#1085#1090#1088#1086#1083#1103
      object gbPP: TGroupBox
        Left = 3
        Top = 8
        Width = 457
        Height = 457
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1087#1086#1087#1077#1088#1077#1095#1085#1099#1093' '#1076#1072#1090#1095#1080#1082#1086#1074
        TabOrder = 0
        object Label15: TLabel
          Left = 290
          Top = 20
          Width = 122
          Height = 19
          Caption = #1056#1077#1078#1080#1084' '#1080#1079#1084#1077#1088#1077#1085#1080#1103
        end
      end
      object gbCross: TGroupBox
        Left = 0
        Top = 471
        Width = 604
        Height = 138
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1087#1086#1087#1077#1088#1077#1095#1085#1086#1075#1086' '#1089#1086#1083#1077#1085#1086#1080#1076#1072
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        object lnumberChannel: TLabel
          Left = 168
          Top = 23
          Width = 93
          Height = 19
          Caption = #1053#1086#1084#1077#1088' '#1082#1072#1085#1072#1083#1072
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
        end
        object lamperage: TLabel
          Left = 8
          Top = 78
          Width = 80
          Height = 19
          Caption = #1057#1086#1083#1077#1085#1086#1080#1076' 1'
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
        end
        object lTemp: TLabel
          Left = 8
          Top = 103
          Width = 82
          Height = 19
          Caption = #1057#1086#1083#1080#1085#1086#1080#1076' 2'
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
        end
        object Label6: TLabel
          Left = 8
          Top = 53
          Width = 95
          Height = 19
          Caption = #1053#1072#1083#1080#1095#1080#1077' '#1087#1086#1083#1103
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
        end
        object Label7: TLabel
          Left = 278
          Top = 55
          Width = 207
          Height = 19
          Caption = #1055#1086#1088#1086#1075' '#1087#1077#1088#1077#1075#1088#1077#1074#1072' '#1089#1086#1083#1077#1085#1086#1080#1076#1072'('#1042')'
        end
        object spSolenoid1: TCSpinEdit
          Left = 168
          Top = 78
          Width = 93
          Height = 24
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 31
          ParentFont = False
          TabOrder = 0
        end
        object spSolenoid2: TCSpinEdit
          Left = 168
          Top = 108
          Width = 93
          Height = 24
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 31
          ParentFont = False
          TabOrder = 1
        end
        object spIsSolenoidsON: TCSpinEdit
          Left = 168
          Top = 48
          Width = 93
          Height = 24
          Font.Charset = RUSSIAN_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = []
          MaxValue = 31
          ParentFont = False
          TabOrder = 2
        end
        object edSolenoidTresholdU: TEdit
          Left = 491
          Top = 48
          Width = 97
          Height = 27
          TabOrder = 3
          Text = '9.0'
        end
      end
      object gbPR: TGroupBox
        Left = 490
        Top = 8
        Width = 457
        Height = 457
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1087#1088#1086#1076#1086#1083#1100#1085#1099#1093' '#1076#1072#1090#1095#1080#1082#1086#1074
        TabOrder = 2
        object Label16: TLabel
          Left = 290
          Top = 20
          Width = 122
          Height = 19
          Caption = #1056#1077#1078#1080#1084' '#1080#1079#1084#1077#1088#1077#1085#1080#1103
        end
      end
    end
    object prPage: TTabSheet
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' LCard 502'
      ImageIndex = 1
      object Label1: TLabel
        Left = 10
        Top = 59
        Width = 182
        Height = 19
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1089#1073#1086#1088#1072' '#1076#1072#1085#1085#1099#1093' ('#1043#1094')'
      end
      object Label3: TLabel
        Left = 19
        Top = 430
        Width = 201
        Height = 19
        Alignment = taCenter
        Caption = #1047#1072#1076#1077#1088#1078#1082#1072' '#1084#1077#1078#1076#1091' '#1082#1072#1076#1088#1072#1084#1080' ('#1084#1089')'
        Visible = False
        WordWrap = True
      end
      object Label2: TLabel
        Left = 10
        Top = 17
        Width = 147
        Height = 19
        Alignment = taCenter
        Caption = #1063#1072#1089#1090#1086#1090#1072' '#1085#1072' '#1082#1072#1085#1072#1083' ('#1043#1094')'
        WordWrap = True
      end
      object eFrequency: TEdit
        Left = 240
        Top = 56
        Width = 182
        Height = 27
        Enabled = False
        TabOrder = 0
      end
      object ppRate: TEdit
        Left = 240
        Top = 11
        Width = 182
        Height = 27
        Enabled = False
        TabOrder = 1
      end
      object ppKadr: TEdit
        Left = 240
        Top = 425
        Width = 182
        Height = 27
        Enabled = False
        TabOrder = 2
        Visible = False
      end
    end
    object OtherPage: TTabSheet
      Caption = #1055#1088#1086#1095#1080#1077' '#1085#1072#1089#1090#1088#1086#1081#1082#1080
      ImageIndex = 2
      object Label4: TLabel
        Left = 8
        Top = 56
        Width = 100
        Height = 19
        Caption = #1058#1086#1082#1086#1074#1099#1081' '#1082#1072#1085#1072#1083
      end
      object Label5: TLabel
        Left = 10
        Top = 104
        Width = 151
        Height = 19
        Caption = #1048#1079#1084#1077#1088#1080#1090#1077#1083#1100#1085#1099#1081' '#1082#1072#1085#1072#1083
      end
      object SpinCurr: TCSpinEdit
        Left = 240
        Top = 48
        Width = 89
        Height = 27
        Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
        Enabled = False
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Times New Roman'
        Font.Style = []
        MaxValue = 31
        ParentFont = False
        TabOrder = 0
      end
      object SpinSG: TCSpinEdit
        Left = 240
        Top = 96
        Width = 89
        Height = 27
        Hint = #1053#1086#1084#1077#1088' '#1089#1086#1086#1090#1074#1077#1090#1089#1090#1074#1091#1102#1097#1077#1075#1086' '#1082#1072#1085#1072#1083#1072', '#1085#1091#1084#1077#1088#1091#1077#1090#1089#1103' '#1089' '#1085#1091#1083#1103
        Enabled = False
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Times New Roman'
        Font.Style = []
        MaxValue = 31
        ParentFont = False
        TabOrder = 1
      end
    end
    object FilterPAge: TTabSheet
      Caption = #1060#1080#1083#1100#1090#1088#1072#1094#1080#1103
      ImageIndex = 4
      object lWidthMedianFilter: TLabel
        Left = 233
        Top = 15
        Width = 200
        Height = 19
        Caption = #1064#1080#1088#1080#1085#1072' '#1084#1077#1076#1080#1072#1085#1085#1086#1075#1086' '#1092#1080#1083#1100#1090#1088#1072
      end
      object cbMedianFilter: TCheckBox
        Left = 24
        Top = 16
        Width = 193
        Height = 17
        Caption = #1052#1077#1076#1080#1072#1085#1085#1072#1103' '#1092#1080#1083#1100#1090#1088#1072#1094#1080#1103
        TabOrder = 0
      end
      object ComboBoxWidthMF: TComboBox
        Left = 456
        Top = 16
        Width = 73
        Height = 27
        Style = csDropDownList
        ItemIndex = 0
        TabOrder = 1
        Text = '3'
        Items.Strings = (
          '3'
          '5'
          '7'
          '9')
      end
      inline FSLineOut: TFilterSettings
        Left = 3
        Top = 44
        Width = 322
        Height = 368
        TabOrder = 2
        ExplicitLeft = 3
        ExplicitTop = 44
        ExplicitWidth = 322
        inherited GroupBox1: TGroupBox
          Caption = #1055#1088#1086#1076#1086#1083#1100#1085#1099#1081', '#1074#1085#1077#1096#1085#1080#1077' '#1076#1077#1092#1077#1082#1090#1099
        end
      end
      inline FSLineIn: TFilterSettings
        Left = 331
        Top = 49
        Width = 322
        Height = 368
        TabOrder = 3
        ExplicitLeft = 331
        ExplicitTop = 49
        ExplicitWidth = 322
        inherited GroupBox1: TGroupBox
          Caption = #1055#1088#1086#1076#1086#1083#1100#1085#1099#1081', '#1074#1085#1091#1090#1088#1077#1085#1085#1080#1077' '#1076#1077#1092#1077#1082#1090#1099
        end
      end
      inline FSCross: TFilterSettings
        Left = 659
        Top = 53
        Width = 322
        Height = 368
        TabOrder = 4
        ExplicitLeft = 659
        ExplicitTop = 53
        ExplicitWidth = 322
        inherited GroupBox1: TGroupBox
          Caption = #1055#1086#1087#1077#1088#1077#1095#1085#1099#1077' '#1076#1077#1092#1077#1082#1090#1099
        end
      end
    end
  end
  object pBottom: TPanel
    Left = 0
    Top = 646
    Width = 996
    Height = 70
    Align = alBottom
    TabOrder = 1
    object bSave: TButton
      Left = 326
      Top = 16
      Width = 113
      Height = 41
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Times New Roman'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = bSaveClick
    end
    object bExit: TButton
      Left = 460
      Top = 16
      Width = 113
      Height = 41
      Caption = #1042#1099#1081#1090#1080
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Times New Roman'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = bExitClick
    end
  end
  object ApplicationEvents: TApplicationEvents
    OnMessage = ApplicationEventsMessage
    Left = 928
    Top = 664
  end
  object CopyValueCrossChannel: TPopupMenu
    Left = 752
    Top = 664
    object CopyAllSensor: TMenuItem
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100' '#1085#1072' '#1076#1088#1091#1075#1080#1077' '#1076#1072#1090#1095#1080#1082#1080
      OnClick = CopyAllSensorClick
    end
  end
  object CopyValueLinearChannel: TPopupMenu
    Left = 624
    Top = 664
    object CopyAllSensorLinear: TMenuItem
      Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100' '#1085#1072' '#1076#1088#1091#1075#1080#1077' '#1076#1072#1090#1095#1080#1082#1080
      OnClick = CopyAllSensorLinearClick
    end
  end
end
