import google.generativeai as genai
import os

api_key = os.getenv("GEMINI_API_KEY")
if not api_key:
    print("Error: GEMINI_API_KEY not set")
    exit(1)

genai.configure(api_key=api_key)

print("Available Gemini Models:")
print("=" * 50)

for model in genai.list_models():
    if 'generateContent' in model.supported_generation_methods:
        print(f"\nModel: {model.name}")
        print(f"  Display Name: {model.display_name}")
        print(f"  Methods: {model.supported_generation_methods}")
