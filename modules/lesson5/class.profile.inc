<?php

  /**
   * This node is new. It is used to edit profiles. A profile is a set of user
   * privileges. In this scenario, each user has one profile (many-to-one
   * relation).
   */
  useattrib("atkdummyattribute");

  /**
   * The profileattribute can be used to edit the privileges.
   */
  useattrib("atkprofileattribute");

  class profile extends atkNode
  {
    function __construct()
    {
      $this->atkNode("profile", NF_EDITAFTERADD|NF_NO_VIEW);

      $this->add(new atkAttribute("id",AF_AUTOKEY));
      $this->add(new atkAttribute("name",AF_OBLIGATORY|AF_UNIQUE|AF_SEARCHABLE, 50));

      /**
       * In lesson4, we already encountered the dummy attribute to display a
       * text. In this lesson, we do the same, but this time we use the atktext()
       * method to internationalize the text. See the en.lng file in the
       * languages/ subdirectory of lesson5, so you can see how the atktext()
       * method translates the string to an actual text.
       */
      $this->add(new atkDummyAttribute("profile_explanation",atktext("profile_explanation", "lesson5"),AF_HIDE_LIST|AF_HIDE_ADD));

      /**
       * The profile attribute edits all application access privileges. The
       * privileges that exist are loaded from the module.inc files. See
       * this modules' module.inc file for an example.
       */
      $this->add(new atkProfileAttribute("accessrights",AF_HIDE_ADD|AF_BLANKLABEL));

      $this->setTable("lesson5_profile");
      $this->setOrder("name");
    }

    function descriptor_def()
    {
      return "[name]";
    }


    function adminFooter()
    {
      return nodeSourceUrl("lesson5.profile");
    }
  }

?>
