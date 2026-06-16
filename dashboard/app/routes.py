from flask import Blueprint, render_template, jsonify, request
from .mqtt_client import devices, water_plant

main = Blueprint("main", __name__)

@main.route("/")
def index():
    return render_template("index.html")

@main.route("/api/data")
def data():
    return jsonify(list(devices.values()))

@main.route("/api/water/<device_id>", methods=["POST"])
def water(device_id):
    water_plant(device_id)
    return jsonify({"status": "sent"})
