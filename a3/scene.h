#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include "veclib.h"
#include "invert.h"

using namespace std;

void slice(const string& str, vector<string>& out) {
	stringstream s;
	s << str;
	string word;
	while (s >> word) {
		out.push_back(word);
	}
}

class Ellipsoid;

struct Ray {
	Vec3 p;
	Vec3 d;

	Ray() : Ray({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0 }) {}
	Ray(Vec3 p, Vec3 d) : p(p), d(d) {}
};

struct Hit {
	Vec3 p;
	Vec3 n;
	double t;
	const Ellipsoid* o;
};

struct Ellipsoid {
	string name;
	Vec3 pos;
	Vec3 scale;
	Vec3 color;
	double Ka;
	double Kd;
	double Ks;
	double Kr;
	int N;
	double T[4][4];
	double Tinv[4][4];

	Ellipsoid(string name, Vec3 pos, Vec3 scale, Vec3 color, double Ka, double Kd, double Ks, double Kr, int N)
		: name(name), pos(pos), scale(scale), color(color), Ka(Ka), Kd(Kd), Ks(Ks), Kr(Kr), N(N)
		, T{ { scale.x, 0.0, 0.0, pos.x }, { 0.0, scale.y, 0.0, pos.y }, { 0.0, 0.0, scale.z, pos.z }, { 0.0, 0.0, 0.0, 1.0 } }
	{
		invert_matrix(T, Tinv);
	}

	static Ellipsoid parse(string str) {
		vector<string> params;
		slice(str, params);
		Ellipsoid res = Ellipsoid(params[0],
			{ stod(params[1]), stod(params[2]), stod(params[3]) },
			{ stod(params[4]), stod(params[5]), stod(params[6]) },
			{ stod(params[7]), stod(params[8]), stod(params[9]) },
			stod(params[10]), stod(params[11]), stod(params[12]), stod(params[13]), stoi(params[14])
		);
		return res;
	}

	bool hit(const Ray& ray, Hit& hit, bool isFirst) const {
		const double offset = isFirst ? 1 : 0.000001;
		Ray invRay = { multiply4(Tinv, ray.p), multiply4(Tinv, ray.d, true) };
		double A = invRay.d.length_squared();
		double B = dot(invRay.p, invRay.d);
		double C = invRay.p.length_squared() - 1;
		double detla = B * B - A * C;
		if (detla < 0) return false;
		double t1 = -(B / A) + sqrt(detla) / A;
		double t2 = -(B / A) - sqrt(detla) / A;
		if (t1 < offset && t2 < offset) return false;
		if (t1 < offset) hit.t = t2;
		else if (t2 < offset) hit.t = t1;
		else hit.t = min(t1, t2);
		Vec3 Pinv = invRay.p + hit.t * invRay.d;
		hit.p = multiply4(T, Pinv);
		double Tinvtar[4][4];
		translate(Tinv, Tinvtar);
		if (dot(ray.d, Pinv) > 0) Pinv = -Pinv;
		hit.n = unit_vector(multiply4(Tinvtar, unit_vector(Pinv), false));
		hit.o = this;
		return true;
	}
};

struct Light {
	string name;
	Vec3 pos;
	Vec3 intensity;

	Light(string name, Vec3 pos, Vec3 intensity): name(name), pos(pos), intensity(intensity) {}

	static Light parse(string str) {
		vector<string> params;
		slice(str, params);
		Light res = Light(params[0], { stod(params[1]), stod(params[2]), stod(params[3]) }, { stod(params[4]), stod(params[5]), stod(params[6]) });
		return res;
	}
};

class Scene {
public:
	double near;
	double left;
	double right;
	double bottom;
	double top;
	int resC;
	int resR;
	Vec3 background;
	Vec3 ambient;
	string output;
	vector<Ellipsoid> ellipsoids;
	vector<Light> lights;

	Scene() {}

	Scene(double near, double left, double right, double bottom, double top, double resC, double resR, Vec3 background, Vec3 ambient, string output)
		: near(near), left(left), right(right), bottom(bottom), top(top), resC(resC), resR(resR), background(background), ambient(ambient), output(output)
	{}

	static Scene parse(string path) {
		Scene scene;
		ifstream ifs;
		ifs.open(path, ios::in);
		if (!ifs) {
			cout << "read file error" << endl;
			return scene;
		}
		char l[100];
		vector<string> words;
		while (!ifs.eof()) {
			words.clear();
			ifs.getline(l, 100);
			string line(l);
			slice(line, words);
			if (words.size() == 0) continue;
			string type = words[0];
			if (type == "NEAR") {
				scene.near = stod(words[1]);
			}
			else if (type == "LEFT") {
				scene.left = stod(words[1]);
			}
			else if (type == "RIGHT") {
				scene.right = stod(words[1]);
			}
			else if (type == "BOTTOM") {
				scene.bottom = stod(words[1]);
			}
			else if (type == "TOP") {
				scene.top = stod(words[1]);
			}
			else if (type == "RES") {
				scene.resC = stoi(words[1]);
				scene.resR = stoi(words[2]);
			}
			else if (type == "SPHERE") {
				scene.ellipsoids.push_back(Ellipsoid(words[1], { stod(words[2]), stod(words[3]), stod(words[4]) }, { stod(words[5]), stod(words[6]), stod(words[7]) }, { stod(words[8]), stod(words[9]), stod(words[10]) }, stod(words[11]), stod(words[12]), stod(words[13]), stod(words[14]), stoi(words[15])));
			}
			else if (type == "LIGHT") {
				scene.lights.push_back(Light(words[1], { stod(words[2]), stod(words[3]), stod(words[4]) }, { stod(words[5]), stod(words[6]), stod(words[7]) }));
			}
			else if (type == "BACK") {
				scene.background = Vec3(stod(words[1]), stod(words[2]), stod(words[3]));
			}
			else if (type == "AMBIENT") {
				scene.ambient = Vec3(stod(words[1]), stod(words[2]), stod(words[3]));
			}
			else if (type == "OUTPUT") {
				scene.output = words[1];
			}
		}
		return scene;
	}

	unsigned char* tracing() const {
		unsigned char* result = new unsigned char[resC * resR * 3];
		double setpV = (top - bottom) / resR;
		double stepH = (right - left) / resC;
		for (int i = 0; i < resR; i++) {
			for (int j = 0; j < resC; j++) {
				Vec3 color = trace({ { 0.0, 0.0, 0.0 }, { left + j * stepH, top - i * setpV, -near } }, 3);
				result[(i * resC + j) * 3] = unsigned char(min(color.x, 1.0) * 255);
				result[(i * resC + j) * 3 + 1] = unsigned char(min(color.y, 1.0) * 255);
				result[(i * resC + j) * 3 + 2] = unsigned char(min(color.z, 1.0) * 255);
			}
		}
		return result;
	}

	Vec3 trace(const Ray& ray, const int& deep) const {
		if (deep <= 0) return { 0.0, 0.0, 0.0 };

		Hit hitMsg;
		bool isHit = hit(ray, hitMsg, deep == 3);
		if (deep == 3 && hitMsg.t < 1) isHit = false;
		if (!isHit) {
			if (deep == 3) {
				return background;
			}
			else {
				return { 0.0, 0.0, 0.0 };
			}
		}

		Ellipsoid obj = *hitMsg.o;
		Vec3 colorA = obj.Ka * ambient * obj.color;
		Vec3 colorL = Vec3();
		for (const Light& light : lights) {
			Vec3 lightDir = light.pos - hitMsg.p;
			Hit _;
			if (!hit({ hitMsg.p, lightDir }, _, false) || _.t > 1) {
				Vec3 diffuse = obj.Kd * light.intensity * max(dot(hitMsg.n, unit_vector(lightDir)), 0.0) * obj.color;
				Vec3 specular = obj.Ks * light.intensity * pow(max(dot(unit_vector(reflect(unit_vector(ray.d), hitMsg.n)), unit_vector(lightDir)), 0.0), obj.N);
				colorL = colorL + diffuse + specular;
			}
		}
		Vec3 colorR = obj.Kr * trace({ hitMsg.p, reflect(ray.d, hitMsg.n) }, deep - 1);

		return colorA + colorL + colorR;
	}

	bool hit(const Ray& ray, Hit& hit, bool isFirst) const {
		hit.t = 999999999;
		bool isHit = false;
		for (const Ellipsoid& ellipsoid : ellipsoids) {
			Hit h;
			bool isH = ellipsoid.hit(ray, h, isFirst);
			if (isH && hit.t > h.t) {
				isHit = true;
				hit = h;
			}
		}
		return isHit;
	}
};
