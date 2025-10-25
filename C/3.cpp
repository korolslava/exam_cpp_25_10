#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <iomanip>
using namespace std;

class MediaFile {
public:
	virtual ~MediaFile() = default;
	virtual string info() const =0;
	virtual double duration() const =0;
};

class Song : public MediaFile {
public:
	Song(string title, string artist, double length)
		: title(move(title)), artist(move(artist)), length(length) {}

	string info() const override {
		return "Song: " + title + " by " + artist + " (" + to_string(length) + " min)";
	}
	double duration() const override { return length; }

private:
	string title;
	string artist;
	double length;
};

class Video : public MediaFile {
public:
	Video(string title, string resolution, double length)
		: title(move(title)), resolution(move(resolution)), length(length) {}

	string info() const override {
		return "Video: " + title + " [" + resolution + "] (" + to_string(length) + " min)";
	}
	double duration() const override { return length; }

private:
	string title;
	string resolution;
	double length;
};

class Podcast : public MediaFile {
public:
	Podcast(string title, string host, double length)
		: title(move(title)), host(move(host)), length(length) {}

	string info() const override {
		return "Podcast: " + title + " hosted by " + host + " (" + to_string(length) + " min)";
	}
	double duration() const override { return length; }

private:
	string title;
	string host;
	double length;
};

class LibraryManager {
public:
	vector<unique_ptr<MediaFile>> mediaFiles;

	void addMediaFile(unique_ptr<MediaFile> mediaFile) {
		mediaFiles.push_back(move(mediaFile));
	}

	double totalDuration() const {
		double sum =0.0;
		for (const auto &m : mediaFiles) sum += m->duration();
		return sum;
	}

	vector<const MediaFile*> filterByMinDuration(double minMinutes) const {
		vector<const MediaFile*> result;
		for (const auto &m : mediaFiles) {
			if (m->duration() >= minMinutes) result.push_back(m.get());
		}
		return result;
	}

	void printAll() const {
		for (const auto &m : mediaFiles) {
			cout << m->info() << "\n";
		}
	}
};

int main()
{
	LibraryManager manager;

	manager.addMediaFile(make_unique<Song>("Yesterday", "The Beatles",2.5));
	manager.addMediaFile(make_unique<Video>("Funny Cats", "1920x1080",10.0));
	manager.addMediaFile(make_unique<Podcast>("TechTalk", "Jane Doe",45.0));

	cout << fixed << setprecision(2);
	cout << "All media files:\n";
	manager.printAll();

	double total = manager.totalDuration();
	cout << "\nTotal duration: " << total << " min\n";

	double minFilter =10.0;
	cout << "\nMedia with duration >= " << minFilter << " min:\n";
	auto filtered = manager.filterByMinDuration(minFilter);
	for (const auto *m : filtered) cout << m->info() << "\n";

	return 0;
}