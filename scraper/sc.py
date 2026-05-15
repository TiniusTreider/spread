import requests
import pandas as pd
import time
import os
import random
from datetime import datetime, timedelta
from tqdm import tqdm

# --- Configuration ---
TICKER_FILE = "../tickers.txt"
# ---------------------

def get_tickers():
    if not os.path.exists(TICKER_FILE):
        print(f"Error: '{TICKER_FILE}' not found.")
        return []
    with open(TICKER_FILE, "r") as f:
        return [line.strip().upper() for line in f if line.strip() and not line.startswith("#")]

# Common browser agents to avoid fingerprinting
USER_AGENTS = [
    'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
    'Mozilla/5.0 (X11; Linux x86_64; rv:121.0) Gecko/20100101 Firefox/121.0',
    'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/17.1 Safari/605.1.15'
]

tickers = get_tickers()

if tickers:
    today = datetime.now()
    start_date_obj = today - timedelta(days=730) # 2 years
    todate = today.strftime("%Y-%m-%d")
    fromdate = start_date_obj.strftime("%Y-%m-%d")

    # Start a persistent session
    session = requests.Session()

    pbar = tqdm(tickers, desc="Scraping Nasdaq", unit="ticker")

    for ticker in pbar:
        pbar.set_description(f"Fetching {ticker}")

        # Update headers with a random User-Agent for every request
        session.headers.update({
            'Accept': 'application/json, text/plain, */*',
            'User-Agent': random.choice(USER_AGENTS),
            'Origin': 'https://www.nasdaq.com',
            'Referer': f'https://www.nasdaq.com/market-activity/stocks/{ticker.lower()}/historical'
        })

        url = f"https://api.nasdaq.com/api/quote/{ticker}/historical?assetclass=stocks&fromdate={fromdate}&limit=9999&todate={todate}"

        try:
            response = session.get(url, timeout=15)

            if response.status_code == 200:
                json_resp = response.json()

                # Check if data exists in the nested JSON structure
                if json_resp.get('data') and json_resp['data'].get('tradesTable'):
                    rows = json_resp['data']['tradesTable']['rows']
                    if rows:
                        df = pd.DataFrame(rows)
                        # Clean currency strings
                        cols_to_fix = ['close', 'high', 'low', 'open']
                        for col in cols_to_fix:
                            if col in df.columns:
                                df[col] = df[col].replace(r'[\$,]', '', regex=True).astype(float)

                        df.to_csv(f"{ticker.lower()}.csv", index=False)
                    else:
                        tqdm.write(f"Empty: {ticker} (No rows found)")
                else:
                    # Log the status message from the API if possible
                    msg = json_resp.get('status', {}).get('bCodeMessage', 'Unknown reason')
                    tqdm.write(f"Skipped {ticker}: {msg}")

            elif response.status_code == 403:
                tqdm.write("Error: 403 Forbidden. You are likely IP-blocked. Stop the script.")
                break
            else:
                tqdm.write(f"Failed {ticker}: HTTP {response.status_code}")

        except Exception as e:
            tqdm.write(f"Error processing {ticker}: {e}")

        # RANDOM SLEEP: Essential for staying under the radar
        # Wait between 4 and 8 seconds
        time.sleep(random.uniform(4, 8))

    print("\nAll downloads complete.")
else:
    print("No tickers to process.")

