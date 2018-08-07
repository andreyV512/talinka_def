object FRLine: TFRLine
  Left = 0
  Top = 0
  Width = 1023
  Height = 201
  TabOrder = 0
  object gbLinearDef: TGroupBox
    Left = 0
    Top = 0
    Width = 1023
    Height = 201
    Align = alClient
    Caption = #1055#1088#1086#1076#1086#1083#1100#1085#1072#1103' '#1076#1077#1092#1077#1082#1090#1086#1089#1082#1086#1087#1080#1103
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentBackground = False
    ParentColor = False
    ParentFont = False
    TabOrder = 0
    object Panel1: TPanel
      Left = 2
      Top = 21
      Width = 1019
      Height = 28
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 0
      OnResize = Panel1Resize
      object Label1: TLabel
        Left = 450
        Top = 5
        Width = 97
        Height = 14
        Caption = #1050#1086#1083'-'#1074#1086'  '#1076#1077#1092'. '#1079#1086#1085
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMenuHighlight
        Font.Height = -12
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        Visible = False
      end
      object cbLinear: TCheckBox
        Left = 11
        Top = 4
        Width = 72
        Height = 15
        Caption = #1042' '#1088#1072#1073#1086#1090#1077
        Checked = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        State = cbChecked
        TabOrder = 0
        OnClick = cbLinearClick
      end
      inline FRBordersLine: TFRBorders
        Left = 90
        Top = 0
        Width = 347
        Height = 22
        TabOrder = 1
        ExplicitLeft = 90
      end
      inline FRDeads1: TFRDeads
        Left = 720
        Top = 2
        Width = 295
        Height = 21
        TabOrder = 2
        Visible = False
        ExplicitLeft = 720
        ExplicitTop = 2
        inherited Label13: TLabel
          Top = 4
          Visible = False
          ExplicitTop = 4
        end
        inherited Label5: TLabel
          Top = 4
          Visible = False
          ExplicitTop = 4
        end
        inherited EFinish: TEdit
          Top = 1
          Visible = False
          ExplicitTop = 1
        end
        inherited EStart: TEdit
          Top = 1
          Visible = False
          ExplicitTop = 1
        end
      end
      object Panel2: TPanel
        Left = 560
        Top = 0
        Width = 37
        Height = 22
        BorderStyle = bsSingle
        Caption = '0'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        Visible = False
      end
    end
    object Chart1: TChart
      Left = 2
      Top = 49
      Width = 1019
      Height = 150
      Legend.Visible = False
      MarginBottom = 0
      MarginLeft = 5
      MarginRight = 5
      MarginTop = 0
      MarginUnits = muPixels
      Title.Text.Strings = (
        'TChart')
      Title.Visible = False
      BottomAxis.Automatic = False
      BottomAxis.AutomaticMaximum = False
      BottomAxis.AutomaticMinimum = False
      BottomAxis.Maximum = 5.000000000000000000
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.LabelsOnAxis = False
      LeftAxis.Maximum = 3.000000000000000000
      LeftAxis.MinorTickCount = 0
      LeftAxis.MinorTickLength = 0
      LeftAxis.TicksInner.Visible = False
      View3D = False
      Zoom.Allow = False
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 1
      ColorPaletteIndex = 13
      object Series1: TBarSeries
        ColorEachPoint = True
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        BarWidthPercent = 100
        Emboss.Color = 8553090
        Emboss.HorizSize = 2
        MultiBar = mbStacked
        Shadow.Color = 8553090
        Shadow.Visible = False
        SideMargins = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          000600000000000000000C82400000000000DC84400000000000D08140000000
          00001484400000000000A886400000000000888840}
      end
      object Series2: TBarSeries
        ColorEachPoint = True
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        BarWidthPercent = 100
        Emboss.Color = 8553090
        Emboss.HorizSize = 2
        MultiBar = mbStacked
        Shadow.Color = 8553090
        Shadow.Visible = False
        SideMargins = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          000600000000000000000C82400000000000DC84400000000000D08140000000
          00001484400000000000A886400000000000888840}
      end
      object Series3: TBarSeries
        ColorEachPoint = True
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        BarWidthPercent = 100
        Emboss.Color = 8487297
        Emboss.HorizSize = 2
        MultiBar = mbStacked
        Shadow.Color = 8487297
        Shadow.Visible = False
        SideMargins = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          000600000000000000000C82400000000000DC84400000000000D08140000000
          00001484400000000000A886400000000000888840}
      end
      object Series4: TBarSeries
        ColorEachPoint = True
        Marks.Arrow.Visible = True
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = True
        Marks.Visible = False
        BarWidthPercent = 100
        Emboss.Color = 8487297
        Emboss.HorizSize = 2
        MultiBar = mbStacked
        Shadow.Color = 8487297
        Shadow.Visible = False
        SideMargins = False
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          000600000000000000000C82400000000000DC84400000000000D08140000000
          00001484400000000000A886400000000000888840}
      end
    end
  end
end
