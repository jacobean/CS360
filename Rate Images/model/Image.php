<?php

function output_iptc_data( $image_path ) {    
    $size = getimagesize ( $image_path, $info);        
     if(is_array($info)) {    
        $iptc = iptcparse($info["APP13"]);
        foreach (array_keys($iptc) as $s) {              
            $c = count ($iptc[$s]);
            for ($i=0; $i <$c; $i++) 
            {
                echo $s.' = '.$iptc[$s][$i].'<br>';
            }
        }                  
    }             
}


class Image {
	protected $title;
	protected $description;
	protected $rating;
	protected $date;
	protected $filename;
	protected $size;

	protected $iptc;

	public function Image($path) {
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

	public function setRating($r) {
		$this->iptc->set(IPTC_PRIORITY, $r);
		$this->iptc->write();
	}

	static function getAll($dir) {
		$images = array();
		$files = scandir($dir);
		foreach ($files as $file) {
			if (substr($file, -3) != 'jpg') continue;
			$images[] = new Image($dir . '/' . $file);
		}

		return $images;
	}

	static function icptToDate($date, $time) {
		return strtotime(preg_replace('/(\d{2})(\d{2})(\d{2}) (\d{2})(\d{2})(\d{2})/', "$1-$2-$3 $4:$5:$6", $date . ' ' . $time));
	}
}