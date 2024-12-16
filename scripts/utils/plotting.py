import plotly.express as px
import pandas as pd

def plot_total_unloads_comparison(df, id_column_name, figure_title, file_save_directory, datetime_str):
    """
    Creates a bar chart using Plotly from df and saves plot to save directory
    """    
    # Create a bar chart using Plotly and save
    fig = px.bar(df, 
                 x=id_column_name, 
                 y='Total Unloads', 
                 title=figure_title, 
                 labels={'id': 'ID', 'unloads': 'Number of Unloads'})
    
    fig.show()
    # fig.write_image(f"{file_save_directory}/{figure_title.replace(' ', '_')}_{datetime_str}.png")

def plot_activity_comparison(df, id_column_name, activity_column_names, figure_title, file_save_directory, datetime_str):
    """
    Creates a grouped bar chart using Plotly from df and saves plot to save directory
    """  
    # Select relevant columns
    df_long = pd.melt(df, id_vars=[id_column_name], 
                      value_vars=activity_column_names,
                      var_name='Activity', value_name='Percentage')
    
    # Plots figure
    fig = px.bar(df_long, 
                 x=id_column_name, 
                 y='Percentage', 
                 color='Activity', 
                 barmode='group', 
                 title=figure_title,
                 labels={'ID': 'ID', 'Percentage': 'Percentage of Total Time (%)'},
                 category_orders={'Activity': activity_column_names})
    
    fig.show()
    # fig.write_image(f"{file_save_directory}/{figure_title.replace(' ', '_')}_{datetime_str}.png")