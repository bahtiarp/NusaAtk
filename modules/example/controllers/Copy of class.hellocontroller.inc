<?php 
 class HelloController extends atkFrontController
  {
    public function helloAction()
    {
      // assign a template variable (method 1)
      $this->title = "Holaaaa!";
      
      // assign another template variable (method 2)
      $this['name'] = $this->request->name;
	  //$this->name = $this->request->name;
    }
  }
  ?>