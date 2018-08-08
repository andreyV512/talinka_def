object ManageForm: TManageForm
  Left = 200
  Top = 108
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #1059#1087#1088#1072#1074#1083#1077#1085#1080#1077' '#1091#1089#1090#1088#1086#1081#1089#1090#1074#1072#1084#1080
  ClientHeight = 193
  ClientWidth = 648
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object gbCross: TGroupBox
    Left = 3
    Top = 8
    Width = 401
    Height = 145
    Caption = #1052#1086#1076#1091#1083#1100' '#1087#1086#1087#1077#1088#1077#1095#1085#1086#1081' '#1076#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1080
    TabOrder = 0
    object lbSolenoidON: TLabel
      Left = 176
      Top = 32
      Width = 192
      Height = 13
      Caption = 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'
    end
    object Label1: TLabel
      Left = 176
      Top = 69
      Width = 73
      Height = 13
      Caption = #1057#1086#1083#1077#1085#1086#1080#1076' 1('#1042')'
    end
    object Label2: TLabel
      Left = 176
      Top = 96
      Width = 73
      Height = 13
      Caption = #1057#1086#1083#1077#1085#1086#1080#1076' 2('#1042')'
    end
    object bCrossSolenoid: TBitBtn
      Left = 10
      Top = 35
      Width = 126
      Height = 49
      Caption = #1042#1082#1083#1102#1095#1080#1090#1100' '#1084#1072#1075#1085#1080#1090#1085#1086#1077' '#1087#1086#1083#1077
      Default = True
      DoubleBuffered = True
      ModalResult = 1
      NumGlyphs = 2
      ParentDoubleBuffered = False
      TabOrder = 0
      WordWrap = True
      OnClick = bCrossSolenoidClick
      OnKeyPress = FormKeyPress
    end
    object edSolenoid1: TEdit
      Left = 264
      Top = 64
      Width = 89
      Height = 21
      Enabled = False
      TabOrder = 1
    end
    object edSolenoid2: TEdit
      Left = 264
      Top = 91
      Width = 89
      Height = 21
      Enabled = False
      TabOrder = 2
    end
  end
  object StatusBarBottom: TStatusBar
    AlignWithMargins = True
    Left = 3
    Top = 159
    Width = 642
    Height = 31
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    Panels = <
      item
        Width = 600
      end>
    ParentColor = True
    UseSystemFont = False
  end
  object GroupBox1: TGroupBox
    Left = 410
    Top = 8
    Width = 231
    Height = 145
    Caption = #1052#1086#1076#1091#1083#1100' '#1087#1088#1086#1076#1086#1083#1100#1085#1086#1081' '#1076#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1080
    TabOrder = 2
    object btRotation: TButton
      Left = 16
      Top = 35
      Width = 121
      Height = 47
      Caption = #1042#1088#1072#1097#1072#1090#1100
      TabOrder = 0
      OnClick = btRotationClick
    end
    object cbRL: TCheckBox
      Left = 176
      Top = 31
      Width = 41
      Height = 17
      Caption = 'RL'
      TabOrder = 1
    end
    object cbRM: TCheckBox
      Left = 176
      Top = 54
      Width = 41
      Height = 17
      Caption = 'RM'
      TabOrder = 2
    end
    object cbRH: TCheckBox
      Left = 176
      Top = 77
      Width = 41
      Height = 17
      Caption = 'RH'
      TabOrder = 3
    end
  end
  object Timer: TTimer
    Enabled = False
    OnTimer = TimerTimer
    Left = 24
    Top = 160
  end
end
