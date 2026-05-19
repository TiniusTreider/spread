# spread

spread downloads historical data for the stocks in S&P 500 and NASDAQ-100 from the last 9 years, and analyzes the data pair-by-pair. It is used to help with pair trading.

## use

Run `make` to compile, and `make clean` to remove files made for the build. Run `./scrape` to get the data from NASDAQ (this takes about 3 hours). Finally, run `./spreadb` to see the analasys.
spread is only supported on Unix.
MSE is the Mean Square Error, and measures how close the stocks keep
Rho_n is the Mean Reversion, and measures how fast the spread closes
Period is how fast the spread closes in trading days (90% close)

