import requests
import pandas as pd
import time
import os
from datetime import datetime, timedelta
from tqdm import tqdm  # Import tqdm

# --- Configuration ---
TICKER_FILE = "../tickers.txt"
# ---------------------

def get_tickers():
    """Reads tickers from a text file, one per line."""
    if not os.path.exists(TICKER_FILE):
        print(f"Error: '{TICKER_FILE}' not found.")
        return []
    with open(TICKER_FILE, "r") as f:
        lines = [line.strip().upper() for line in f if line.strip() and not line.startswith("#")]
    return lines

tickers = get_tickers()

if tickers:
    today = datetime.now()
    start_date_obj = today - timedelta(days=730)
    todate = today.strftime("%Y-%m-%d")
    fromdate = start_date_obj.strftime("%Y-%m-%d")

    headers = {
        'Accept': 'application/json, text/plain, */*',
        'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
        'Origin': 'https://www.nasdaq.com',
        'Referer': 'https://www.nasdaq.com/'
    }

    # Use tqdm as a wrapper around the tickers list
    # desc: Label on the left | unit: Label for each item
    pbar = tqdm(tickers, desc="Scraping Nasdaq", unit="ticker")

    for ticker in pbar:
        # Update the description dynamically to show the current ticker
        pbar.set_description(f"Fetching {ticker}")

        url = f"https://api.nasdaq.com/api/quote/{ticker}/historical?assetclass=stocks&fromdate={fromdate}&limit=9999&todate={todate}"

        try:
            response = requests.get(url, headers=headers, timeout=15)

            if response.status_code == 200:
                json_resp = response.json()

                if json_resp.get('data') and json_resp['data'].get('tradesTable'):
                    rows = json_resp['data']['tradesTable']['rows']
                    df = pd.DataFrame(rows)

                    cols_to_fix = ['close', 'high', 'low', 'open']
                    for col in cols_to_fix:
                        if col in df.columns:
                            df[col] = df[col].replace(r'[\$,]', '', regex=True).astype(float)

                    filename = f"{ticker.lower()}.csv"
                    df.to_csv(filename, index=False)
                else:
                    # Use tqdm.write instead of print to avoid breaking the bar
                    tqdm.write(f"Skipping {ticker}: No data returned.")
            else:
                tqdm.write(f"Failed {ticker}: HTTP {response.status_code}")

        except Exception as e:
            tqdm.write(f"Error processing {ticker}: {e}")

        time.sleep(2.5)

    print("\nAll downloads complete.")
else:
    print("No tickers to process. Exiting.")

