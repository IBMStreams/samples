## DSPFilterFinite Sample

This sample demonstrates how to use the **DSPFilterFinite** operator. Unlike the **DSPFilter** operator, which performs filtering against single, continuous time series, the **DSPFilterFinite** operator is capable of filtering finite-length time series segments. This is useful where only segments of a time series need to be filtered or whether each incoming tuple contains a list of a complete time series.
