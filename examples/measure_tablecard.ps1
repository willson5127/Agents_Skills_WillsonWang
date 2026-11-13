Add-Type -AssemblyName System.Drawing
$source = Get-Content -LiteralPath (Join-Path $PSScriptRoot 'tablecard-demo.json') -Raw -Encoding UTF8 | ConvertFrom-Json
$bitmap = [System.Drawing.Bitmap]::new(1, 1)
$graphics = [System.Drawing.Graphics]::FromImage($bitmap)
$regular = [System.Drawing.Font]::new('Arial', 11, [System.Drawing.FontStyle]::Regular, [System.Drawing.GraphicsUnit]::Pixel)
$bold = [System.Drawing.Font]::new('Arial', 11, [System.Drawing.FontStyle]::Bold, [System.Drawing.GraphicsUnit]::Pixel)
$titleFont = [System.Drawing.Font]::new('Arial', 10.5, [System.Drawing.FontStyle]::Bold, [System.Drawing.GraphicsUnit]::Pixel)
$result = @{}
try {
    foreach ($obj in $source.objects) {
        $title = "$($source.database).$($obj.schema).$($obj.name)【$($obj.label)】"
        $widths = @{}
        foreach ($column in $obj.columns) {
            $widths[$column.name] = [Math]::Ceiling($graphics.MeasureString($column.name, $regular).Width)
        }
        $result["$($obj.schema).$($obj.name)"] = @{
            title = [Math]::Ceiling($graphics.MeasureString($title, $titleFont).Width)
            names = $widths
            key = [Math]::Ceiling($graphics.MeasureString('UK1', $bold).Width)
        }
    }
    $result | ConvertTo-Json -Depth 5 -Compress
} finally {
    $titleFont.Dispose(); $bold.Dispose(); $regular.Dispose(); $graphics.Dispose(); $bitmap.Dispose()
}
