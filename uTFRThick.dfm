object FRThick: TFRThick
  Left = 0
  Top = 0
  Width = 770
  Height = 295
  TabOrder = 0
  OnResize = FrameResize
  object gbThickness: TGroupBox
    Left = 0
    Top = 0
    Width = 770
    Height = 295
    Align = alClient
    Caption = #1050#1086#1085#1090#1088#1086#1083#1100' '#1090#1086#1083#1097#1080#1085#1099
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Label1: TLabel
      Left = 450
      Top = 23
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
    object Label2: TLabel
      Left = 605
      Top = 23
      Width = 82
      Height = 14
      Caption = #1052#1080#1085'. '#1058#1086#1083#1097#1080#1085#1072
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMenuHighlight
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      Visible = False
    end
    object Chart1: TChart
      Left = 2
      Top = 43
      Width = 706
      Height = 250
      BackWall.Color = clBtnFace
      BackWall.Transparent = False
      BottomWall.Visible = False
      Foot.AdjustFrame = False
      Foot.Alignment = taLeftJustify
      Foot.Visible = False
      LeftWall.Visible = False
      Legend.Visible = False
      MarginBottom = 0
      MarginLeft = 5
      MarginRight = 5
      MarginTop = 0
      MarginUnits = muPixels
      PrintProportional = False
      SubFoot.Alignment = taLeftJustify
      SubFoot.Visible = False
      SubTitle.Alignment = taLeftJustify
      SubTitle.Visible = False
      Title.Alignment = taLeftJustify
      Title.Text.Strings = (
        '')
      Title.Visible = False
      BottomAxis.Automatic = False
      BottomAxis.AutomaticMaximum = False
      BottomAxis.AutomaticMinimum = False
      BottomAxis.Axis.Width = 1
      BottomAxis.ExactDateTime = False
      BottomAxis.Increment = 1.000000000000000000
      BottomAxis.LabelsSeparation = 0
      BottomAxis.Maximum = 8.000000000000000000
      BottomAxis.MaximumOffset = 1
      BottomAxis.MinorTickCount = 0
      DepthAxis.Automatic = False
      DepthAxis.AutomaticMaximum = False
      DepthAxis.AutomaticMinimum = False
      DepthAxis.ExactDateTime = False
      DepthAxis.Increment = 1.000000000000000000
      DepthTopAxis.Automatic = False
      DepthTopAxis.AutomaticMaximum = False
      DepthTopAxis.AutomaticMinimum = False
      DepthTopAxis.ExactDateTime = False
      DepthTopAxis.Increment = 1.000000000000000000
      Emboss.Color = clLime
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.ExactDateTime = False
      LeftAxis.Increment = 1.000000000000000000
      LeftAxis.LabelsMultiLine = True
      LeftAxis.Maximum = 10.000000000000000000
      LeftAxis.MinorTicks.Visible = False
      Pages.AutoScale = True
      Pages.MaxPointsPerPage = 7
      RightAxis.Automatic = False
      RightAxis.AutomaticMaximum = False
      RightAxis.AutomaticMinimum = False
      RightAxis.Visible = False
      Shadow.Color = clRed
      Shadow.Visible = False
      TopAxis.Automatic = False
      TopAxis.AutomaticMaximum = False
      TopAxis.AutomaticMinimum = False
      TopAxis.Visible = False
      View3D = False
      Zoom.Allow = False
      BevelOuter = bvNone
      TabOrder = 0
      PrintMargins = (
        15
        15
        15
        45)
      ColorPaletteIndex = 13
      object Series3: TBarSeries
        ColorEachPoint = True
        Depth = 0
        Marks.Arrow.Visible = False
        Marks.Callout.Brush.Color = clBlack
        Marks.Callout.Arrow.Visible = False
        Marks.Margins.Left = 44
        Marks.Visible = False
        ShowInLegend = False
        AutoMarkPosition = False
        BarRound = brNone
        BarWidthPercent = 100
        Dark3D = False
        MarksOnBar = True
        MultiBar = mbStacked
        OffsetPercent = 50
        Shadow.Visible = False
        YOrigin = 3.000000000000000000
        XValues.Name = 'X'
        XValues.Order = loAscending
        YValues.Name = 'Bar'
        YValues.Order = loNone
        Data = {
          0007000000000000000050894000000000004887400000000000A88640000000
          000008864000000000006885400000000000F084400000000000D08140}
      end
    end
    inline FRBordersThick: TFRBorders
      Left = 90
      Top = 21
      Width = 359
      Height = 22
      TabOrder = 1
      ExplicitLeft = 90
      ExplicitTop = 21
      ExplicitWidth = 359
    end
    object Panel1: TPanel
      Left = 560
      Top = 18
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
      TabOrder = 2
      Visible = False
    end
    object Panel2: TPanel
      Left = 695
      Top = 18
      Width = 60
      Height = 22
      BorderStyle = bsSingle
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
end
