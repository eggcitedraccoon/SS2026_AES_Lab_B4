from flask import Blueprint, render_template, jsonify
from .mqtt_client import latest_data

main = Blueprint("main", __name__)

@main.route("/")
def index():
    return render_template("index.html")

@main.route("/api/data")
def data():
    return jsonify(latest_data)
