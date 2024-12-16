import argparse
import logging
import os
import sys
from datetime import datetime
from utils.plotting import *
from utils.file_io import *

# Set up logging
def setup_logging(log_file="app.log"):
    """
    Sets up logging configuration.

    Args:
        log_file (str): Path to the log file.
    """
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s [%(levelname)s] %(message)s",
        handlers=[
            logging.FileHandler(log_file),
            logging.StreamHandler(sys.stdout)
        ]
    )

# Parse command-line arguments
def parse_arguments():
    """
    Parses command-line arguments.

    Returns:
        argparse.Namespace: Parsed arguments.
    """
    parser = argparse.ArgumentParser(description="Mining Simulation Peformance Visualization")
    parser.add_argument("mining_trucks_csv_filepath", type=str, help="Path to the csv containing mining truck performances.")
    parser.add_argument("unloading_stations_csv_filepath", type=str, help="Path to the csv containing unloading station performances.")
    parser.add_argument("--verbose", action="store_true", help="Enable verbose logging")
    return parser.parse_args()


# Main functionality
def main():
    # ****IF MORE TIME: Would create more varied charts to show vehicle time split between mining, travel, 
    # and unload (i.e pie chart, etc)

    # Parse arguments
    args = parse_arguments()

    # Validate arguments
    truck_csv_path = args.mining_trucks_csv_filepath
    station_csv_path = args.unloading_stations_csv_filepath

    if not os.path.exists(truck_csv_path):
        logging.error(f"Error: File '{truck_csv_path}' does not exist.")
        sys.exit(1)  # Exit with error code

    if not os.path.exists(station_csv_path):
        logging.error(f"Error: File '{station_csv_path}' does not exist.")
        sys.exit(1)  # Exit with error code

    # Set up logging
    log_directory = "results/visualizer_script_logs"
    datetime_str = datetime.now().strftime('%Y%m%d_%H%M%S')
    log_file = os.path.join(log_directory, f"MiningSimVisulizer_{datetime_str}.log")
    os.makedirs(log_directory, exist_ok=True)  # Create logs directory if doesnt exist
    setup_logging(log_file)

    # Set logging level based on verbosity
    if args.verbose:
        logging.getLogger().setLevel(logging.DEBUG)

    logging.info("Generating mining simulation performance visualizations....")
    logging.info("Mining Truck Plots...")

    ## Create directory to save plots
    plot_save_directory = "results/plots"
    os.makedirs(plot_save_directory, exist_ok=True)  # Create logs directory if doesnt exist

    ## Read CSV Data to pandas dataframe
    mining_truck_df = import_csv_to_dataframe(truck_csv_path)
    plot_total_unloads_comparison(mining_truck_df, 'Vehicle ID', 'Total Number of Unloads by Vehicle ID', plot_save_directory, datetime_str)
    plot_activity_comparison(mining_truck_df, 'Vehicle ID',['Percent Mining Time', 'Percent Unloading Time', 'Percent Travel Time'],
                            'Percentage of Time Spent on Mining, Unloading, and Travel by Vehicle', plot_save_directory, datetime_str)

    logging.info("Unloading Station Plots...")

    ## Read CSV Data to pandas dataframe
    unloading_station_df = import_csv_to_dataframe(station_csv_path)
    plot_total_unloads_comparison(unloading_station_df, 'Station ID', 'Total Number of Unloads by Station ID', plot_save_directory, datetime_str)
    plot_activity_comparison(unloading_station_df, 'Station ID',['Percent Idle Time', 'Percent Unloading Time'],
                            'Percentage of Time Spent in Idle and Unloading States by Station', plot_save_directory, datetime_str)
    
    logging.info("Done!")

if __name__ == "__main__":
    main()