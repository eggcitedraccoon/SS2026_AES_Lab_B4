from flask import Flask
from .mqtt_client import init_mqtt

def create_app():
    app = Flask(__name__)
    app.config.from_object("app.config.Config")

    from .routes import main
    app.register_blueprint(main)

    # Start MQTT client
    init_mqtt(app)

    return app
