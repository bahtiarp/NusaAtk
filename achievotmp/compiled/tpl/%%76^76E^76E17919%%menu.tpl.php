<?php /* Smarty version 2.6.31, created on 2025-01-23 09:06:51
         compiled from ./themes/nusaatk/templates/menu.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'atkconfig', './themes/nusaatk/templates/menu.tpl', 1, false),array('function', 'atkthemeimg', './themes/nusaatk/templates/menu.tpl', 14, false),)), $this); ?>
<?php echo smarty_function_atkconfig(array('var' => 'companylogo','smartyvar' => 'theme_logo'), $this);?>

<?php echo smarty_function_atkconfig(array('var' => 'admin_logo','smartyvar' => 'admin_logo'), $this);?>

<?php echo smarty_function_atkconfig(array('var' => 'persondir_url','smartyvar' => 'persondir'), $this);?>

<?php echo smarty_function_atkconfig(array('var' => 'client_logo_compressed','smartyvar' => 'dir_logo'), $this);?>

<?php echo smarty_function_atkconfig(array('var' => 'user_photo_compressed','smartyvar' => 'dir_foto'), $this);?>

<?php echo smarty_function_atkconfig(array('var' => 'client_name','smartyvar' => 'client_name'), $this);?>

<?php echo smarty_function_atkconfig(array('var' => 'pp','smartyvar' => 'pp'), $this);?>




<aside class="app-sidebar bg-body-secondary shadow" data-bs-theme="light"> <!--begin::Sidebar Brand-->

    <div class="sidebar-brand"> <!--begin::Brand Link--> <a href="./" class="brand-link">
            <!--begin::Brand Image--> <img src="<?php echo smarty_function_atkthemeimg(array('id' => "logo.png"), $this);?>
" alt="NusaATK Logo"
                class="brand-image opacity-75 "> <!--end::Brand Image--> <!--begin::Brand Text--> 
                <!-- <span class="brand-text fw-light">AdminLTE 4</span>  -->
            </div>
    <!--end::Sidebar Brand--> <!--begin::Sidebar Menu-->

    <div class="sidebar-wrapper" data-overlayscrollbars="host">
        <div data-overlayscrollbars-viewport="scrollbarHidden" style="margin-right: -16px; margin-bottom: -16px; margin-left: 0px; top: -8px; right: auto; left: -8px; width: calc(100% + 16px); padding: 8px;">
            <nav class="fw-bold"> <!--begin::Sidebar Menu-->
                <ul class="nav sidebar-menu flex-column" data-lte-toggle="treeview" role="menu" data-accordion="false">
                    <?php echo $this->_tpl_vars['content']; ?>

                </ul>
            </nav> <!--end::Sidebar Menu-->
        </div>
    </div>
</aside> <!--end::Sidebar Brand--> <!--begin::Sidebar Menu-->

<main class="app-main"> <!--begin::App Content Header-->
    