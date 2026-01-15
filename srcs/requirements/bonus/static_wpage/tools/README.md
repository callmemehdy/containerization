# AI Assistant Service

A lightweight AI chatbot powered by Google Gemini API, built with Streamlit.

## Features

- Interactive chat interface
- Powered by Google Gemini Pro
- Clean and modern UI
- Chat history management
- Multi-stage Docker build for minimal image size

## Setup

1. Get your Gemini API key from: https://makersuite.google.com/app/apikey

2. Add the API key to your `.env` file:
```bash
GEMINI_API_KEY=your_api_key_here
```

3. Build and run:
```bash
make up
```

4. Access the app at: http://localhost:8080

## Architecture

- Framework: Streamlit
- AI Model: Google Gemini 2.5 Flash
- Python: 3.11-slim
- Multi-stage build: Optimized for size (~200MB)

## Docker Image Size Optimization

The Dockerfile uses multi-stage builds:
1. Builder stage: Compiles dependencies with gcc
2. Final stage: Only runtime dependencies, no build tools
3. Result: Minimal image size with fast startup

## Environment Variables

| Variable | Description | Required |
|----------|-------------|----------|
| GEMINI_API_KEY | Your Google Gemini API key | Yes |

## Usage

- Type your question in the chat input
- Press Enter to send
- Click "Clear Chat" to reset conversation

## Security Notes

- Never commit your API key to git
- Keep `.env` file private
- Use `.env.example` as template
