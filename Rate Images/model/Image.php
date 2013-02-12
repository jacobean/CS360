<?php

define('IMAGES_BASE_PATH', 'public/images/uploaded/');

class Image {
	protected $title;
	protected $description;
	protected $rating;
	protected $date;
	protected $filename;
	protected $size;

	protected $iptc;

	public function Image($filename) {
		$path = IMAGES_BASE_PATH . $filename;

		$this->iptc = new iptc($path);

		$this->filename = basename($path);
		$this->size = getimagesize($path, $info);

		$cdate = $this->iptc->get(IPTC_CREATED_DATE);
		$ctime = $this->iptc->get(IPTC_CREATED_TIME);
		$this->date = $this::icptToDate($cdate, $ctime);

		$this->title = $this->iptc->get(IPTC_OBJECT_NAME);
		$this->description = $this->iptc->get(IPTC_CAPTION);

		$this->rating = $this->iptc->get(IPTC_PRIORITY);

		return;
	}

	public function getTitle() { return $this->title; }
	public function getDescription() { return $this->description; }
	public function getFilename() { return $this->filename; }
	public function getRating() { return $this->rating; }
	public function getDate() { return $this->date; }
	public function getSize() { return $this->size; }

	public function setTitle($title) {
		$this->title = $title;
		$this->iptc->set(IPTC_OBJECT_NAME, $title);
		$this->iptc->write();
	}

	public function setDescription($description) {
		$this->description = $description;
		$this->iptc->set(IPTC_CAPTION, $description);
		$this->iptc->write();
	}

	public function setDate() {
		$now = time();
		$this->date = $now;
		$this->iptc->set(IPTC_CREATED_DATE, date('ymd', $now));
		$this->iptc->set(IPTC_CREATED_TIME, date('His', $now));
		$this->iptc->write();
	}

	public function setRating($r) {
		$this->rating = $r;
		$this->iptc->set(IPTC_PRIORITY, $r);
		$this->iptc->write();
	}

	public function incRating() {
		$this->setRating($this->rating + 1);
	}

	public function decRating() {
		$this->setRating($this->rating - 1);
	}

	static function getAll() {
		$images = array();
		$files = scandir(IMAGES_BASE_PATH);
		foreach ($files as $file) {
			if (
				substr($file, -3) != 'jpg' &&
				substr($file, -4) != 'jpeg'
			) continue;

			$image = new Image($file);

			$images[] = $image;
		}

		return $images;
	}

	static function icptToDate($date, $time) {
		return strtotime(preg_replace('/(\d{2})(\d{2})(\d{2}) (\d{2})(\d{2})(\d{2})/', "$1-$2-$3 $4:$5:$6", $date . ' ' . $time));
	}
}