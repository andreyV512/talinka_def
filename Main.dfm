object MainForm: TMainForm
  Left = -44
  Top = 0
  Caption = #1041#1059#1056#1040#1053'-5000  '#8470'1627. '#1052#1072#1075#1085#1080#1090#1086#1080#1085#1076#1091#1082#1094#1080#1086#1085#1085#1072#1103' '#1076#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1103' '#1053#1050#1058
  ClientHeight = 802
  ClientWidth = 1276
  Color = clBtnFace
  Constraints.MinHeight = 840
  Constraints.MinWidth = 700
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clBlack
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyPress = FormKeyPress
  OnResize = MainFormInit
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object bTest: TButton
    Left = 10
    Top = 74
    Width = 120
    Height = 41
    Caption = #1058#1077#1089#1090
    DropDownMenu = OtherTest
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlue
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    Style = bsSplitButton
    TabOrder = 6
    OnClick = bTestClick
    OnKeyPress = FormKeyPress
  end
  object bView: TButton
    Left = 136
    Top = 74
    Width = 120
    Height = 41
    Caption = #1055#1088#1086#1089#1084#1086#1090#1088
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlue
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = bViewClick
    OnKeyPress = FormKeyPress
  end
  object bWork: TButton
    Left = 262
    Top = 74
    Width = 124
    Height = 41
    Caption = #1056#1072#1073#1086#1090#1072
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlue
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    OnClick = bWorkClick
    OnKeyPress = FormKeyPress
  end
  object StatusBarBottom: TStatusBar
    AlignWithMargins = True
    Left = 3
    Top = 768
    Width = 1270
    Height = 31
    Panels = <
      item
        Width = 320
      end
      item
        Text = #1043#1086#1090#1086#1074' '#1082' '#1088#1072#1073#1086#1090#1077
        Width = 400
      end
      item
        Width = 300
      end
      item
        Width = 50
      end>
  end
  object StatusBarTop: TStatusBar
    Left = 0
    Top = 26
    Width = 1284
    Height = 42
    Align = alNone
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    Panels = <
      item
        Text = ' '#1058#1080#1087#1086#1088#1072#1079#1084#1077#1088':'
        Width = 230
      end
      item
        Width = 300
      end
      item
        Width = 50
      end>
    Touch.ParentTabletOptions = False
    Touch.TabletOptions = []
    UseSystemFont = False
  end
  object cbTypeSize: TComboBox
    Left = 107
    Top = 32
    Width = 113
    Height = 27
    AutoDropDown = True
    AutoCloseUp = True
    Style = csDropDownList
    Ctl3D = False
    DoubleBuffered = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ImeMode = imDisable
    ParentCtl3D = False
    ParentDoubleBuffered = False
    ParentFont = False
    TabOrder = 0
    TabStop = False
    Touch.InteractiveGestures = [igPressAndTap]
    Touch.ParentTabletOptions = False
    Touch.TabletOptions = [toPressAndHold]
    OnKeyPress = FormKeyPress
    OnSelect = cbTypeSizeSelect
  end
  object bManage: TButton
    Left = 525
    Top = 74
    Width = 120
    Height = 41
    Caption = #1053#1072#1083#1072#1076#1082#1072
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlue
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = bManageClick
    OnKeyPress = FormKeyPress
  end
  object bCancelWork: TButton
    Left = 396
    Top = 74
    Width = 120
    Height = 41
    Caption = #1054#1090#1084#1077#1085#1072
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clBlue
    Font.Height = 20
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = bCancelWorkClick
    OnKeyPress = FormKeyPress
  end
  object cbInterruptView: TCheckBox
    Left = 156
    Top = 129
    Width = 147
    Height = 17
    Caption = #1055#1088#1077#1088#1099#1074#1072#1085#1080#1077' '#1085#1072' '#1087#1088#1086#1089#1084#1086#1090#1088
    TabOrder = 4
  end
  inline FRCross1: TFRCross
    Left = 9
    Top = 157
    Width = 744
    Height = 100
    TabOrder = 9
    ExplicitLeft = 9
    ExplicitTop = 157
    ExplicitWidth = 744
    ExplicitHeight = 100
    inherited gbCrossDef: TGroupBox
      Width = 744
      Height = 100
      ExplicitHeight = 100
      inherited Chart1: TChart
        Width = 740
        Height = 49
        BottomAxis.ExactDateTime = False
        BottomAxis.Increment = 1.000000000000000000
        BottomAxis.LabelsSeparation = 0
        DepthAxis.Automatic = False
        DepthAxis.AutomaticMaximum = False
        DepthAxis.AutomaticMinimum = False
        DepthAxis.Maximum = 2.809999999999996000
        DepthAxis.Minimum = 1.809999999999999000
        DepthTopAxis.Automatic = False
        DepthTopAxis.AutomaticMaximum = False
        DepthTopAxis.AutomaticMinimum = False
        DepthTopAxis.Maximum = 2.809999999999996000
        DepthTopAxis.Minimum = 1.809999999999999000
        LeftAxis.ExactDateTime = False
        LeftAxis.Increment = 5.000000000000000000
        LeftAxis.LabelsSeparation = 0
        LeftAxis.MinorTicks.Visible = False
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        ExplicitHeight = 49
        inherited Series1: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
        inherited Series2: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
        inherited Series3: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
        inherited Series4: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
        inherited Series5: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
        inherited Series6: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
        inherited Series7: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
        inherited Series8: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
        inherited Series9: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
        inherited Series10: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
        inherited Series11: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
        inherited Series12: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
      end
      inherited Panel1: TPanel
        Width = 740
      end
    end
  end
  inline FRLine1: TFRLine
    Left = 2
    Top = 246
    Width = 756
    Height = 130
    TabOrder = 10
    ExplicitLeft = 2
    ExplicitTop = 246
    ExplicitWidth = 756
    ExplicitHeight = 130
    inherited gbLinearDef: TGroupBox
      Width = 756
      Height = 130
      ExplicitWidth = 756
      ExplicitHeight = 130
      inherited Panel1: TPanel
        Width = 752
        ExplicitWidth = 752
        inherited FRDeads1: TFRDeads
          inherited EStart: TEdit
            Top = -1
            ExplicitTop = -1
          end
        end
      end
      inherited Chart1: TChart
        Width = 752
        Height = 79
        MarginLeft = 10
        BottomAxis.ExactDateTime = False
        BottomAxis.Increment = 1.000000000000000000
        BottomAxis.LabelsSeparation = 0
        BottomAxis.MinorTicks.Visible = False
        DepthAxis.Automatic = False
        DepthAxis.AutomaticMaximum = False
        DepthAxis.AutomaticMinimum = False
        DepthAxis.Maximum = 2.150000000000000000
        DepthAxis.Minimum = 1.150000000000001000
        DepthTopAxis.Automatic = False
        DepthTopAxis.AutomaticMaximum = False
        DepthTopAxis.AutomaticMinimum = False
        DepthTopAxis.Maximum = 2.150000000000000000
        DepthTopAxis.Minimum = 1.150000000000001000
        LeftAxis.ExactDateTime = False
        LeftAxis.Increment = 1.000000000000000000
        LeftAxis.LabelsSeparation = 0
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        ExplicitWidth = 752
        ExplicitHeight = 79
        inherited Series1: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
        inherited Series2: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
        inherited Series3: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
        inherited Series4: TBarSeries
          Data = {
            000600000000000000000C82400000000000DC84400000000000D08140000000
            00001484400000000000A886400000000000888840}
        end
      end
    end
  end
  inline FRThick1: TFRThick
    Left = 2
    Top = 383
    Width = 710
    Height = 100
    TabOrder = 11
    ExplicitLeft = 2
    ExplicitTop = 383
    ExplicitWidth = 710
    ExplicitHeight = 100
    inherited gbThickness: TGroupBox
      Width = 710
      Height = 100
      ExplicitHeight = 100
      inherited Chart1: TChart
        Height = 55
        LeftAxis.LabelsOnAxis = False
        LeftAxis.Maximum = 28.000000000000040000
        LeftAxis.Minimum = 18.000000000000030000
        ExplicitHeight = 55
        PrintMargins = (
          15
          15
          15
          45)
        inherited Series3: TBarSeries
          Data = {
            0007000000000000000050894000000000004887400000000000A88640000000
            000008864000000000006885400000000000F084400000000000D08140}
        end
      end
    end
  end
  inline FRSum1: TFRSum
    Left = 2
    Top = 487
    Width = 1054
    Height = 130
    TabOrder = 12
    ExplicitLeft = 2
    ExplicitTop = 487
    ExplicitHeight = 130
    inherited gbSummary: TGroupBox
      Height = 130
      ExplicitHeight = 130
      inherited Chart1: TChart
        Height = 83
        MarginLeft = 20
        BottomAxis.ExactDateTime = False
        BottomAxis.Increment = 1.000000000000000000
        BottomAxis.LabelsSeparation = 0
        LeftAxis.Labels = False
        ExplicitHeight = 83
      end
      inherited Panel1: TPanel
        inherited pSolidGroup: TPanel
          Visible = False
        end
      end
    end
  end
  object Button1: TButton
    Left = 8
    Top = 121
    Width = 75
    Height = 25
    Caption = '502 test'
    TabOrder = 13
    Visible = False
  end
  object ActionMainMenuBar1: TActionMainMenuBar
    Left = 0
    Top = 0
    Width = 1284
    Height = 26
    UseSystemFont = False
    ActionManager = ActionManager1
    Align = alNone
    Caption = 'ActionMainMenuBar1'
    Color = clMenuBar
    ColorMap.HighlightColor = clWhite
    ColorMap.UnusedColor = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -12
    Font.Name = 'Segoe UI'
    Font.Style = []
    Spacing = 0
  end
  inline FRStatist1: TFRStatist
    Left = 655
    Top = 72
    Width = 179
    Height = 74
    TabOrder = 15
    ExplicitLeft = 655
    ExplicitTop = 72
    ExplicitWidth = 179
    ExplicitHeight = 74
    inherited GroupBox1: TGroupBox
      Width = 179
      Height = 74
      ExplicitWidth = 179
      ExplicitHeight = 74
    end
  end
  inline FRSOP1: TFRSOP
    Left = 309
    Top = 129
    Width = 313
    Height = 22
    TabOrder = 16
    ExplicitLeft = 309
    ExplicitTop = 129
    inherited cbEtalonView: TCheckBox
      OnClick = FRSOP1cbEtalonViewClick
    end
    inherited cbSelectEtalon: TComboBox
      OnDropDown = nil
    end
  end
  object WinWorkPanel: TPanel
    Left = 156
    Top = 584
    Width = 491
    Height = 169
    BorderStyle = bsSingle
    TabOrder = 17
    Visible = False
    object lWinWorkMessage: TLabel
      Left = 25
      Top = 31
      Width = 439
      Height = 46
      Alignment = taCenter
      Caption = 
        #1056#1077#1082#1086#1084#1077#1085#1076#1091#1077#1084' '#1074#1072#1084' '#1087#1077#1088#1077#1079#1072#1075#1088#1091#1079#1080#1090#1100' '#1082#1086#1084#1087#1100#1102#1090#1077#1088' - '#1086#1085' '#1088#1072#1073#1086#1090#1072#1077#1090' '#1089#1083#1080#1096#1082#1086#1084' '#1076#1086 +
        #1083#1075#1086
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      WordWrap = True
    end
    object bWinWorkOk: TButton
      Left = 188
      Top = 103
      Width = 117
      Height = 49
      Caption = 'OK'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = bWinWorkOkClick
    end
  end
  object SaveToFileDialog: TSaveDialog
    DefaultExt = '.dkb'
    Filter = #1044#1072#1085#1085#1099#1077' '#1087#1086' '#1090#1088#1091#1073#1077'(.dkb)|*.dkb|'#1042#1089#1077' '#1092#1072#1081#1083#1099'|*.*'
    Options = [ofHideReadOnly, ofNoChangeDir, ofShowHelp, ofEnableSizing]
    Left = 912
  end
  object OpenDialogFromFile: TOpenDialog
    DefaultExt = '.dkb'
    Filter = #1044#1072#1085#1085#1099#1077' '#1087#1086' '#1090#1088#1091#1073#1077'(.dkb)|*.dkb'
    Options = [ofHideReadOnly, ofNoChangeDir, ofEnableSizing]
    Left = 1008
  end
  object ApplicationEvents: TApplicationEvents
    OnMessage = ApplicationEventsMessage
    Left = 816
  end
  object OtherTest: TPopupMenu
    BiDiMode = bdLeftToRight
    ParentBiDiMode = False
    Left = 680
    object TestRotation: TMenuItem
      Caption = #1058#1077#1089#1090' '#1089' '#1074#1088#1072#1097#1077#1085#1080#1077#1084
    end
  end
  object XPManifest1: TXPManifest
    Left = 616
    Top = 8
  end
  object ActionManager1: TActionManager
    ActionBars = <
      item
        Items = <
          item
            Items = <
              item
                Action = Action1
                Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1090#1080#1087#1086#1088#1072#1079#1084#1077#1088#1072
              end
              item
                Action = Action2
                Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1080#1079#1084#1077#1088#1077#1085#1080#1081
              end
              item
                Action = Action3
                Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1094#1074#1077#1090#1072
              end>
            Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072
          end
          item
            Items = <
              item
                Action = Action5
                Caption = #1055#1088#1086#1089#1084#1086#1090#1088
              end
              item
                Action = Action7
                Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
              end
              item
                Action = Action8
                Caption = #1054#1090#1082#1088#1099#1090#1100
              end>
            Caption = #1058#1088#1091#1073#1072
          end
          item
            Items = <
              item
                Action = Action9
                Caption = #1058#1077#1089#1090
              end
              item
                Visible = False
                Action = Action10
                Caption = #1058#1077#1089#1090' '#1091#1089#1090#1088#1086#1081#1089#1090#1074
              end
              item
                Visible = False
                Action = Action6
                Caption = #1058#1077#1089#1090' '#1089#1074#1103#1079#1080' '#1089' '#1052#1072#1088#1082#1077#1088#1086#1084
              end
              item
                Action = Action12
                Caption = #1057#1080#1075#1085#1072#1083#1099
              end
              item
                Action = Action13
                Caption = #1055#1088#1086#1090#1086#1082#1086#1083
              end>
            Caption = #1044#1080#1072#1075#1085#1086#1089#1090#1080#1082#1072
          end
          item
            Items = <
              item
                Action = Action17
                Caption = #1043#1088#1072#1092#1080#1082#1080' '#1043#1055
              end>
            Visible = False
            Caption = #1043#1088#1091#1087#1087#1072' '#1087#1088#1086#1095#1085#1086#1089#1090#1080
          end
          item
            Items = <
              item
                Action = Action21
                Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
              end>
            Caption = #1055#1086#1084#1086#1097#1100
          end
          item
            Items = <
              item
                Action = Action25
                Caption = #1057#1074#1077#1088#1082#1072' '#1057#1054#1055
              end
              item
                Action = Action24
                Caption = #1046#1091#1088#1085#1072#1083' '#1082#1086#1085#1090#1088#1086#1083#1103
              end
              item
                Action = Action23
                Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1057#1054#1055
              end
              item
                Action = Action22
                Caption = #1044#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1089#1090#1099
              end>
            Caption = #1050#1086#1085#1090#1088#1086#1083#1100' '#1057#1054#1055
          end>
        ActionBar = ActionMainMenuBar1
      end>
    Left = 8
    Top = 616
    StyleName = 'Platform Default'
    object Action1: TAction
      Caption = 'Action1'
      OnExecute = menuTypeSizeClick
    end
    object Action2: TAction
      Caption = 'Action2'
      OnExecute = menuMeasuresClick
    end
    object Action3: TAction
      Caption = 'Action3'
      OnExecute = menuColorsClick
    end
    object Action4: TAction
      Caption = 'Action4'
    end
    object Action5: TAction
      Caption = 'Action5'
      OnExecute = bViewClick
    end
    object Action7: TAction
      Caption = 'Action7'
      OnExecute = menuSaveTubeClick
    end
    object Action8: TAction
      Caption = 'Action8'
      OnExecute = menuLoadTubeClick
    end
    object Action9: TAction
      Caption = 'Action9'
      OnExecute = bTestClick
    end
    object Action10: TAction
      Caption = 'Action10'
      OnExecute = menuTestAdvantechClick
    end
    object Action11: TAction
      Caption = 'Action11'
    end
    object Action12: TAction
      Caption = 'Action12'
      OnExecute = menuSignalsStateClick
    end
    object Action13: TAction
      Caption = 'Action13'
      OnExecute = menuProtocolClick
    end
    object Action14: TAction
      Caption = 'Action14'
    end
    object Action15: TAction
      Caption = 'Action15'
    end
    object Action16: TAction
      Caption = 'Action16'
    end
    object Action17: TAction
      Caption = 'Action17'
    end
    object Action18: TAction
      Caption = 'Action18'
    end
    object Action19: TAction
      Caption = 'Action19'
    end
    object Action20: TAction
      Caption = 'Action20'
      OnExecute = menuF1Click
    end
    object Action21: TAction
      Caption = 'Action21'
      OnExecute = N1Click
    end
    object Action6: TAction
      Caption = 'Action6'
      Visible = False
    end
    object Action22: TAction
      Caption = 'Action22'
      OnExecute = Action22Execute
    end
    object Action23: TAction
      Caption = 'Action23'
      OnExecute = Action23Execute
    end
    object Action24: TAction
      Caption = 'Action24'
      OnExecute = Action24Execute
    end
    object Action25: TAction
      Caption = 'Action25'
      OnExecute = Action25Execute
    end
  end
  object WinWorkTimer: TTimer
    OnTimer = WinWorkTimerTimer
    Left = 696
    Top = 672
  end
end
