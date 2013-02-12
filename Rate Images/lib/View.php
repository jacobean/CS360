<?php
class View {
    protected $template_dir = 'view/';
    protected $vars = array();
    public function __construct($template_dir = null) {
        if ($template_dir !== null) {
            // Check here whether this directory really exists
            $this->template_dir = $template_dir;
        }
    }
    public function render($template_file, $use_layout = true) {
        if (file_exists($this->template_dir . $template_file)) {
            if ($use_layout) {
                ob_start();
                include $this->template_dir . $template_file;
                $this->content = ob_get_clean();
                include $this->template_dir . 'layout.phtml';
            } else {
                include $this->template_dir . $template_file;
            }
        } else {
            throw new Exception('no template file ' . $template_file . ' present in directory ' . $this->template_dir);
        }
    }
    public function __set($name, $value) {
        $this->vars[$name] = $value;
    }
    public function __get($name) {
        return isset($this->vars[$name]) ? $this->vars[$name] : null;
    }
}
?>