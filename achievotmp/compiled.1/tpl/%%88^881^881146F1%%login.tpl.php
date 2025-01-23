<?php /* Smarty version 2.6.31, created on 2024-10-23 11:54:35
         compiled from ./themes/nusaatk/templates/login.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'atkconfig', './themes/nusaatk/templates/login.tpl', 1, false),array('function', 'atkthemeimg', './themes/nusaatk/templates/login.tpl', 4, false),array('function', 'atktext', './themes/nusaatk/templates/login.tpl', 35, false),)), $this); ?>
<?php echo smarty_function_atkconfig(array('var' => 'theme_logo','smartyvar' => 'login_logo'), $this);?>

<?php echo smarty_function_atkconfig(array('var' => 'client_name','smartyvar' => 'client_name'), $this);?>

<?php if (! isset ( $this->_tpl_vars['login_logo'] )): ?><?php echo smarty_function_atkconfig(array('var' => 'theme_logo','smartyvar' => "nusasms-logo.png"), $this);?>
<?php endif; ?>
<?php if (! isset ( $this->_tpl_vars['login_logo'] )): ?><?php ob_start(); ?><?php echo smarty_function_atkthemeimg(array('id' => "kwk-jaklingko.png"), $this);?>
<?php $this->_smarty_vars['capture']['default'] = ob_get_contents();  $this->assign('login_logo', ob_get_contents());ob_end_clean(); ?><?php endif; ?>



<div class="container">

    <div class="left">
        <img class="logo" style="width:200px;" src="<?php echo smarty_function_atkthemeimg(array('id' => "logo.png"), $this);?>
" alt="">
        <h2 style="color:#215f9a;" class="title">PHP Business Application Framework</h2>
        <img  class="angkot" src="<?php echo smarty_function_atkthemeimg(array('id' => "line-sketch.png"), $this);?>
" alt="">
    </div>
    <div class="right">
        <div class="row">
            <div class="col-12">
                <div class="login-form">
                    <h4>Login</h4>
                    <form id="login_form" name="login_form" method="post" action="<?php echo $this->_tpl_vars['formurl']; ?>
" autocomplete="off">
                        <?php if (isset ( $this->_tpl_vars['auth_max_loginattempts_exceeded'] )): ?>
                        <?php echo $this->_tpl_vars['auth_max_loginattempts_exceeded']; ?>

                        <?php else: ?>
                        <?php echo $this->_tpl_vars['atksessionformvars']; ?>

                        <?php if (isset ( $this->_tpl_vars['auth_mismatch'] )): ?><div class="alert alert-danger"><?php echo $this->_tpl_vars['auth_mismatch']; ?>
</div><?php endif; ?>
                        <?php if (isset ( $this->_tpl_vars['auth_account_locked'] )): ?><div class="alert alert-danger"><?php echo $this->_tpl_vars['auth_account_locked']; ?>
</div>
                        <?php endif; ?>
                        <?php if (isset ( $this->_tpl_vars['auth_mismatch_recaptcha'] )): ?><div class="alert alert-danger"><?php echo $this->_tpl_vars['auth_mismatch_recaptcha']; ?>

                        </div><?php endif; ?>

                        <input tabindex="1" id="username" type="text"  name="auth_user"
                            placeholder="Username" required autofocus />
                            <input tabindex="2" id="password" type="password"  name="auth_pw" placeholder="Password" required />
                            <?php if ($this->_tpl_vars['auth_enablepasswordmailer']): ?>
                            <a href="/recover"><?php echo smarty_function_atktext(array('id' => 'password_forgotten'), $this);?>
</a>
                        <?php endif; ?>
                        <button tabindex="3" class="btn btn-primary">Sign In</button>
                        <?php endif; ?>
                    </form>
                    <a href="./rst_pass?forgot-pass=true">Forgot password?</a>
                </div>
            </div>
        </div>
    </div>
</div>