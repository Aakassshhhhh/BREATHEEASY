# Install dependencies (run once)
!pip install datasets scikit-learn pandas

# Import libraries
import pandas as pd
from datasets import load_dataset
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_absolute_error, mean_squared_error, r2_score
import numpy as np

# -----------------------------
# 1️⃣ LOAD YOUR DATASET
# -----------------------------
# Replace 'your_file.csv' with your actual CSV filename
your_data = pd.read_csv('your_file.csv')

# Assume your dataset has columns like: 'PM2.5', 'Temperature', 'Humidity', 'AQI'
# Adjust this line according to your columns
X_your = your_data[['PM2.5', 'Temperature', 'Humidity']]
y_your = your_data['AQI']

# -----------------------------
# 2️⃣ LOAD PUBLIC DATASET FROM HUGGING FACE
# -----------------------------
# Example: Delhi Air Quality Dataset
dataset = load_dataset("abhinavsarkar/delhi_air_quality_feature_store", split='train')
public_data = pd.DataFrame(dataset)

# Select similar columns (rename if necessary)
X_public = public_data[['pm25', 'temperature', 'humidity']].dropna()
y_public = public_data['aqi'].loc[X_public.index]

# -----------------------------
# 3️⃣ TRAIN/TEST SPLIT
# -----------------------------
Xy_train, Xy_test, yy_train, yy_test = train_test_split(X_your, y_your, test_size=0.2, random_state=42)
Xp_train, Xp_test, yp_train, yp_test = train_test_split(X_public, y_public, test_size=0.2, random_state=42)

# -----------------------------
# 4️⃣ TRAIN LINEAR REGRESSION ON BOTH
# -----------------------------
model_your = LinearRegression()
model_public = LinearRegression()

model_your.fit(Xy_train, yy_train)
model_public.fit(Xp_train, yp_train)

# -----------------------------
# 5️⃣ PREDICT & EVALUATE
# -----------------------------
def evaluate_model(model, X_test, y_test, name):
    y_pred = model.predict(X_test)
    mae = mean_absolute_error(y_test, y_pred)
    rmse = np.sqrt(mean_squared_error(y_test, y_pred))
    r2 = r2_score(y_test, y_pred)
    print(f"📊 {name} RESULTS")
    print(f"MAE: {mae:.2f}, RMSE: {rmse:.2f}, R²: {r2:.2f}\n")
    return mae, rmse, r2

mae_y, rmse_y, r2_y = evaluate_model(model_your, Xy_test, yy_test, "Your Dataset")
mae_p, rmse_p, r2_p = evaluate_model(model_public, Xp_test, yp_test, "Public (Delhi) Dataset")

# -----------------------------
# 6️⃣ CREATE COMPARISON TABLE
# -----------------------------
comparison = pd.DataFrame({
    "Metric": ["MAE", "RMSE", "R² Score"],
    "Your Dataset": [mae_y, rmse_y, r2_y],
    "Public Dataset": [mae_p, rmse_p, r2_p]
})

print("\n📈 COMPARISON TABLE")
print(comparison.to_string(index=False))
