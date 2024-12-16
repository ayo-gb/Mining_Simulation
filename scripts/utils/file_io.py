import pandas as pd
import logging

def import_csv_to_dataframe(file_path):
    """
    Reads a CSV file and imports its data into a pandas DataFrame.
    
    Args:
        file_path (str): Path to the CSV file.

    Returns:
        pd.DataFrame: A DataFrame containing the CSV data.
    """
    try:
        # Read the CSV file into a DataFrame
        df = pd.read_csv(file_path)
        return df
    except pd.errors.EmptyDataError:
        logging.error(f"Error: The file '{file_path}' is empty.")
    except pd.errors.ParserError as e:
        logging.error(f"Error parsing the CSV file: {e}")
    except Exception as e:
        logging.error(f"An unexpected error occurred: {e}")