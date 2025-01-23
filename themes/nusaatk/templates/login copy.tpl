{atkconfig var="theme_logo" smartyvar="login_logo"}
{atkconfig var="client_name" smartyvar="client_name"}
{if !isset($login_logo)}{atkconfig var="theme_logo" smartyvar="nusasms-logo.png"}{/if}
{if !isset($login_logo)}{capture assign="login_logo"}{atkthemeimg id="kwk-jaklingko.png"}{/capture}{/if}



<div class="container" style="padding-top:100px;">
    <div class="row">
        <div class="col-sm-8 col-sm-offset-2"><br>
            <div class="text-center" >
                <img style="padding-bottom:30px;" width="30%" class="logo_perusahaan1" border="0" src="{if empty($login_logo)}{atkthemeimg id="kwk-jaklingko.png"}{else}{$login_logo}{/if}" alt="Logo" />
            </div>
            <div class="panel panel-default">
                <div class="panel-body">
                    <form id="login_form" name="login_form" method="post" action="{$formurl}" autocomplete="off">
                        <h1 style="margin-top: 0;">Sign In</h1>
                        <div class="row">
                            <div class="col-sm-7">
                                {if isset($auth_max_loginattempts_exceeded)}
                                    {$auth_max_loginattempts_exceeded}
                                {else}
                                    {$atksessionformvars}
                                    {if isset($auth_mismatch)}<div class="alert alert-danger">{$auth_mismatch}</div>{/if}
                                    {if isset($auth_account_locked)}<div class="alert alert-danger">{$auth_account_locked}</div>{/if}
                                    {if isset($auth_mismatch_recaptcha)}<div class="alert alert-danger">{$auth_mismatch_recaptcha}</div>{/if}
                                    <div class="form">
                                        <div class="form-group">
                                            <!-- <label>Username</label>  -->
                                            <input tabindex="1" id="username" type="text" class="form-control" name="auth_user" placeholder="Username" required autofocus />
                                        </div>
                                        <div class="form-group">
                                            <!-- <label>Password</label>  -->
                                            <input tabindex="2" id="password" type="password" class="form-control" name="auth_pw" placeholder="Password" required />
                                            {if $auth_enablepasswordmailer}
                                                <a href="/recover">{atktext id=password_forgotten}</a>
                                            {/if}
                                        </div>
                                        {*<div class="form-group">
                                        <div class="g-recaptcha" data-sitekey="6LfxWQcTAAAAAEwP6oJVCO1mOYR4AHLy0AjMlUag"></div>
                                        </div>*}
                                        <div class="form-group">
                                            <div class="row">
                                                <div class="col-xs-6 col-sm-6 col-md-6 col-lg-6">
                                                    <button tabindex="3" class="btn btn-primary">Sign In</button>
                                                </div>
                                                <div class="col-xs-6 col-sm-6 col-md-6 col-lg-6 text-right">
                                                    <a class="btn btn-success" tabindex="4" data-toggle="modal" href="http://nusasms.com/freetrial/" id="signup_id">Sign Up</a>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            {/if}
                            <div class="col-sm-5" >
 <img src="./images/kwk/mobil.png" alt="" class="active" style="max-height: 200px;">
                            </div>
                        </div>
                    </form>
                </div>
            </div>
            <div class="text-center">

            </div>
        </div>
    </div>
</div>

<div class="modal fade" id="signup_modal">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-body">
                <div id="alert"></div>
                <div id="signup_modal_body">
                    <form class="form-horizontal" role="form" id="signup_form">
                        <div class="form-group">
                            <label for="name" class="col-sm-2 control-label">Name<sup><small><i class="fa fa-asterisk text-danger"></i></small></sup></label>
                            <div class="col-sm-10">
                                <input type="text" class="form-control" id="name" placeholder="Enter your name" maxlength="50" name="name" required autofocus>
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="phone" class="col-sm-2 control-label">Phone<sup><small><i class="fa fa-asterisk text-danger"></i></small></sup></label>
                            <div class="col-sm-10">
                                <input type="number" class="form-control" id="phone" placeholder="Enter your phone" maxlength="15" name="phone" required>
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="email" class="col-sm-2 control-label">Email<sup><small><i class="fa fa-asterisk text-danger"></i></small></sup></label>
                            <div class="col-sm-10">
                                <input type="email" class="form-control" id="email" placeholder="Enter your email" maxlength="30" name="email" required>
                            </div>
                        </div>
                        <div class="form-group">
                            <label for="company" class="col-sm-2 control-label">Company</label>
                            <div class="col-sm-10">
                                <input type="text" class="form-control" id="company" maxlength="80" placeholder="Your company" name="company">
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-sm-offset-2 col-sm-10">
                                <button type="submit" class="btn btn-danger" id="signup_btn">Sign up now</button>
                                <span id='loader'><img src="./images/loader.gif"/></span>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>                                                    

</body>
</html>
