#include "ofApp.h"

ofApp::~ofApp() {

	for (ofxBulletBox* tmp : this->bounds) {
		delete tmp;
	}

	for (ofxBulletBox* tmp : this->boxes) {
		delete tmp;
	}

	this->bounds.clear();
	this->boxes.clear();
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowTitle("Bullet");
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);

	//camera.setPosition(ofVec3f(0, -500.f, -500.f));
	//camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, 1, 0));

	world.setup();
	world.enableCollisionEvents();
	//ofAddListener(world.COLLISION_EVENT, this, &ofApp::onCollision);
	world.setGravity(ofVec3f(0, 1024, 0));
	//world.enableDebugDraw();
	world.setCamera(&camera);

	camera.setTarget(ofVec3f(0, -512, 0));

	ofxBulletBox* bound = new ofxBulletBox();
	bound->create(world.world, ofVec3f(0.f, 0.f, 0.f), 0.0f, 4096.0, 5.0, 4096.0);
	bound->setProperties(0.35, 0.95);
	bound->add();
	this->bounds.push_back(bound);

	this->pre_position = ofVec3f(0, 0, 0);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofVec3f power(0, 0, 0);
	Leap::Frame frame = leap.frame();
	Leap::HandList hands = frame.hands();

	this->grab = false;
	world.setGravity(ofVec3f(0, 1024, 0));
	for (int i = 0; i < hands.count(); i++) {
		if (hands[i].grabStrength() > 0.8 && hands[i].isRight()) {

			power = ofVec3f(hands[i].palmPosition().x - this->pre_position.x, hands[i].palmPosition().y - this->pre_position.y, hands[i].palmPosition().z - this->pre_position.z);
			power *= 64;
			this->pre_position = ofVec3f(hands[i].palmPosition().x, hands[i].palmPosition().y, hands[i].palmPosition().z);
			cout << "x = " << power.x << " y = " << power.y << " z = " << power.z << endl;

			for (int i = 0; i < this->boxes.size(); i++) {
				this->boxes[i]->applyCentralForce(power);
			}

			this->grab = true;
			world.setGravity(ofVec3f(0, 0, 0));
		}
	}

	if (!this->grab) {
		for (int i = 0; i < 5; i++) {
			ofxBulletBox* box = new ofxBulletBox();
			box->create(world.world, ofVec3f(ofRandom(-512, 512), -1024, ofRandom(-512, 512)), 1.0f, 15.0, 15.0, 15.0);
			box->setRestitution(1.0);
			box->applyTorque(ofRandom(-100, 100), ofRandom(-100, 100), ofRandom(-100, 100));
			box->add();
			this->boxes.push_back(box);

			ofColor color;
			color.setHsb(ofRandom(255), 255, 255);
			this->boxes_color.push_back(color);

			this->boxes_life.push_back(300);
		}

		vector<int> removeIndexes = {};
		for (int i = 0; i < this->boxes.size(); i++) {
			this->boxes_life[i] -= 1;

			if (this->boxes_life[i] < 0) {
				this->boxes[i]->remove();
				removeIndexes.push_back(i);
			}
		}

		for (int i = removeIndexes.size() - 1; i > -1; i--) {
			this->boxes_life.erase(this->boxes_life.begin() + removeIndexes[i]);
			this->boxes_color.erase(this->boxes_color.begin() + removeIndexes[i]);
			this->boxes.erase(this->boxes.begin() + removeIndexes[i]);
		}
	}

	world.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	glEnable(GL_DEPTH_TEST);

	camera.begin();

	ofSetLineWidth(1.f);
	ofSetColor(255, 0, 200);

	//world.drawDebug();

	for (int i = 0; i < this->boxes.size(); i++) {
		ofSetColor(this->boxes_color[i], this->boxes_life[i]);
		this->boxes[i]->draw();
	}


	for (int i = 0; i < this->bounds.size(); i++) {
		ofSetColor(128);
		this->bounds[i]->draw();
	}

	camera.end();

	ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::onCollision(ofxBulletCollisionData& cdata) {

}