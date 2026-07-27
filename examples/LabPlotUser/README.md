# Recreate the LabPlot demo yourself

This folder is the learning path. The finished project is in
[`../LabPlotDemo`](../LabPlotDemo/). First run one of its scripts so that a
live CSV stream is available, then recreate the project in LabPlot.

## 1. Start a producer

Use `../LabPlotDemo/run_csv.sh` for the direct CSV path, or
`../LabPlotDemo/run_trice.sh` for the binary Trice path. Both end at the same
UDP stream `127.0.0.1:9000`, one numeric record per datagram:

```text
0.000000,0.000000,1.000000,0.000000
```

Leave the script running while configuring LabPlot.

## 2. Add the live data source

In LabPlot create a new project and choose **Add New > Live Data Source**.
Select **Network UDP Socket**, enter host `127.0.0.1` and port `9000`, and
select the ASCII filter. Use comma as separator, disable header detection,
set all four data types to `Double`, and enter the names `time_s`, `x`, `y`,
and `z`. Select **Update on new data** and retain **500 values**.

Save the project as `LabPlotUser.lml`. These settings are the important part:
the producer is continuous, and retaining 500 rows gives a moving ten-second
window at the demo rate.

## 3. Create the time plot

Add a worksheet and a Cartesian plot. Add three XY curves using the live
source. For all three curves choose `time_s` as the X column; choose `x`, `y`,
and `z` as the Y column. Show the legend and label the axes `time [s]` and
`value`. Enable automatic range scaling, select **Last values**, and enter
`500`. This keeps the horizontal resolution constant while the plot moves
through the newest ten seconds.

## 4. Create the Lissajous plot

Add a second Cartesian plot to the same worksheet, select a horizontal
two-column layout, and add one XY curve with `x` as the X column and `y` as
the Y column. Use fixed symmetric ranges around `-1.1` to `1.1` if you want a
stable circular view. Select **Last values** and enter `150` so the plot draws
a moving three-second trace from the same live source. The slow phase drift in
both producers keeps the Lissajous figure changing visibly.

## 5. Try the other producer

Close the running producer and start the other script. Do not change the
LabPlot project: both producers are intentionally normalized to the same CSV
stream. If no data appears, check that only one producer is using UDP port
`9000` and that the LabPlot source is still connected. The Trice script waits
for LabPlot to open this port, then waits for the decoder to open its internal
UDP port `9001` before it starts the producer.

For a longer or shorter history, change the live source's retained-value count
from `500` to `50 * seconds`. The Trice script uses UDP port `9001` internally
before forwarding the normalized stream to port `9000`.
