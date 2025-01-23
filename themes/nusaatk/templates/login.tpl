{atkconfig var="theme_logo" smartyvar="login_logo"}
{atkconfig var="client_name" smartyvar="client_name"}
{if !isset($login_logo)}{atkconfig var="theme_logo" smartyvar="nusasms-logo.png"}{/if}
{if !isset($login_logo)}{capture assign="login_logo"}{atkthemeimg id="kwk-jaklingko.png"}{/capture}{/if}



<div class="container">

    <div class="left">
        <img class="logo" style="width:200px;" src="{atkthemeimg id="logo.png"}" alt="">
        <h2 style="color:#215f9a;" class="title">PHP Business Application Framework</h2>
        <img  class="angkot" src="{atkthemeimg id="line-sketch.png"}" alt="">
    </div>
    <div class="right">
        <div class="row">
            <div class="col-12">
                <div class="login-form">
                    <h4>Login</h4>
                    <form id="login_form" name="login_form" method="post" action="{$formurl}" autocomplete="off">
                        {if isset($auth_max_loginattempts_exceeded)}
                        {$auth_max_loginattempts_exceeded}
                        {else}
                        {$atksessionformvars}
                        {if isset($auth_mismatch)}<div class="alert alert-danger">{$auth_mismatch}</div>{/if}
                        {if isset($auth_account_locked)}<div class="alert alert-danger">{$auth_account_locked}</div>
                        {/if}
                        {if isset($auth_mismatch_recaptcha)}<div class="alert alert-danger">{$auth_mismatch_recaptcha}
                        </div>{/if}

                        <input tabindex="1" id="username" type="text"  name="auth_user"
                            placeholder="Username" required autofocus />
                            <input tabindex="2" id="password" type="password"  name="auth_pw" placeholder="Password" required />
                            {if $auth_enablepasswordmailer}
                            <a href="/recover">{atktext id=password_forgotten}</a>
                        {/if}
                        <button tabindex="3" class="btn btn-primary">Sign In</button>
                        {/if}
                    </form>
                    <a href="./rst_pass?forgot-pass=true">Forgot password?</a>
                </div>
            </div>
        </div>
    </div>
</div>