import streamlit as st
import google.generativeai as genai
import os

st.set_page_config(
    page_title="AI Assistant",
    page_icon="🔧",
    layout="centered"
)

api_key = os.getenv("GEMINI_API_KEY")

if not api_key:
    st.error("GEMINI_API_KEY not found in environment variables!")
    st.stop()

genai.configure(api_key=api_key)

st.title("AI Assistant")
st.caption("Powered by Google Gemini")

if "messages" not in st.session_state:
    st.session_state.messages = []

for message in st.session_state.messages:
    with st.chat_message(message["role"]):
        st.markdown(message["content"])

if prompt := st.chat_input("Ask me anything..."):
    st.session_state.messages.append({"role": "user", "content": prompt})
    
    with st.chat_message("user"):
        st.markdown(prompt)
    
    with st.chat_message("assistant"):
        message_placeholder = st.empty()
        
        try:
            model = genai.GenerativeModel('models/gemini-2.5-flash')
            response = model.generate_content(prompt)
            full_response = response.text
            
            message_placeholder.markdown(full_response)
            st.session_state.messages.append({"role": "assistant", "content": full_response})
            
        except Exception as e:
            error_msg = f"Error: {str(e)}"
            message_placeholder.markdown(error_msg)
            st.session_state.messages.append({"role": "assistant", "content": error_msg})

if st.session_state.messages:
    if st.button("Clear Chat"):
        st.session_state.messages = []
        st.rerun()
