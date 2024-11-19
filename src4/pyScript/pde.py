from bs4 import BeautifulSoup
import pandas as pd

# Load the HTML PMD report
html_path = '/Users/zitian/pmd-bin-7.7.0/Rxjava_pmd_report1.html'
important_folders = [
    'jmh/java/io/reactivex/rxjava3/core',
    'main/java/io/reactivex/rxjava3/core',
    'test/java/io/reactivex/rxjava3/core'
]

# Read and parse the HTML file
with open(html_path, 'r') as file:
    content = file.read()
soup = BeautifulSoup(content, 'html.parser')

# Extract table rows, filtering violations by important folders
violations = []
for row in soup.find_all('tr')[1:]:  # Skipping the header row
    cols = row.find_all('td')
    if len(cols) > 3:
        file_path = cols[1].get_text(strip=True)
        problem = cols[3].get_text(strip=True)
        
        # Check if the file path contains any of the important folders
        for folder in important_folders:
            if folder in file_path:
                violations.append([folder, problem])
                break

# Convert the data to a DataFrame
df = pd.DataFrame(violations, columns=['Folder', 'Violation Type'])

# Summarize violations by counting occurrences for each type and folder
summary_df = df.groupby(['Folder', 'Violation Type']).size().unstack(fill_value=0)

# Print the summary table
print(summary_df)

# Optionally, save to CSV for further use
summary_df.to_csv('pmd_violation_summary.csv')