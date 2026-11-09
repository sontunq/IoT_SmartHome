// Giao dien web cua Cerberus - HTML thuan, sua truc tiep o day (hoac sua index.html roi dan vao)
#pragma once
const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!doctype html><html><head><meta charset=utf8><meta name=viewport content="width=device-width,initial-scale=1,viewport-fit=cover"><style>:root{color-scheme:light;box-sizing:border-box;padding-top:env(safe-area-inset-top,0px);padding-bottom:env(safe-area-inset-bottom,0px)}html{scroll-padding-top:env(safe-area-inset-top,0px)}body{margin:0;padding:0;font:14px -apple-system,BlinkMacSystemFont,sans-serif;background:#fff;color:#000}img{max-width:100%}[hidden]:not([hidden=until-found i]){display:none!important}</style></head><body>
<title>Cerberus</title>
<meta name="claude:viewport-fit-sides" content="cover">
<link rel="preconnect" href="https://fonts.googleapis.com"><link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
<link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Chakra+Petch:wght@600;700&family=Be+Vietnam+Pro:wght@400;500;600;700&family=JetBrains+Mono:wght@500&display=swap">
<style>
/* Bố cục: thanh bên (máy tính) / thanh tab dưới (điện thoại) · trang Tổng quan là lưới thẻ: cửa + robot, trong nhà, đèn, nhiệt độ, màn LCD, hoạt động */
:root{
  --bg:#edf0f2;--panel:#ffffff;--panel2:#f4f6f8;--ink:#14212b;--mute:#5c6a76;--line:#d9e0e5;
  --accent:#0b8a99;--accent-ink:#ffffff;--accent-soft:#e1f2f4;
  --good:#1d7f47;--good-soft:#e3f3e9;--warn:#a8650a;--warn-soft:#fbefdc;--crit:#c3302f;--crit-soft:#fbe5e4;
  --oled:#8ff0f8;--bezel:#0f151a;--lcd:#2b63c9;
  --surface:var(--panel);--surface2:var(--panel2);--mono:var(--f-mono);--wall:#dde3e8;--inside:#2a3744;--door:#b98b5a;--door2:#a67b4d;--frame:#8d99a5;
  --f-disp:"Chakra Petch","Be Vietnam Pro",system-ui,sans-serif;--f-body:"Be Vietnam Pro",system-ui,-apple-system,"Segoe UI",sans-serif;--f-mono:"JetBrains Mono",ui-monospace,SFMono-Regular,Menlo,monospace;
}
@media (prefers-color-scheme:dark){:root:not([data-theme="light"]){--bg:#0c1216;--panel:#141d23;--panel2:#1a252c;--ink:#e4ecf0;--mute:#93a2ad;--line:#25323a;--accent:#2bb3c2;--accent-ink:#06161a;--accent-soft:#12303a;--good:#4cc47f;--good-soft:#123222;--warn:#e7a54b;--warn-soft:#3a2a12;--crit:#f06b67;--crit-soft:#3d1918;--wall:#1f2a34;--inside:#0a0f14;--door:#9b7449;--door2:#8b6741;--frame:#52616e;color-scheme:dark}}
:root[data-theme="dark"]{--bg:#0c1216;--panel:#141d23;--panel2:#1a252c;--ink:#e4ecf0;--mute:#93a2ad;--line:#25323a;--accent:#2bb3c2;--accent-ink:#06161a;--accent-soft:#12303a;--good:#4cc47f;--good-soft:#123222;--warn:#e7a54b;--warn-soft:#3a2a12;--crit:#f06b67;--crit-soft:#3d1918;--wall:#1f2a34;--inside:#0a0f14;--door:#9b7449;--door2:#8b6741;--frame:#52616e;color-scheme:dark}
*{box-sizing:border-box}
[hidden]{display:none!important}
body{background:var(--bg);color:var(--ink);font-family:var(--f-body);font-size:14.5px;line-height:1.5;margin:0}
button{font:inherit;color:inherit}
:focus-visible{outline:2px solid var(--accent);outline-offset:2px}
.app{display:grid;grid-template-columns:232px minmax(0,1fr);min-height:100vh}
/* ---- thanh bên ---- */
.side{position:sticky;top:0;height:100vh;padding:22px 14px;border-right:1px solid var(--line);background:var(--panel);display:flex;flex-direction:column;gap:18px}
.brand{display:flex;align-items:center;gap:10px;padding:0 8px}
.brand .logo{width:34px;height:34px;border-radius:10px;background:var(--bezel);display:grid;place-items:center}
.brand b{font-family:var(--f-disp);font-size:20px;letter-spacing:.02em}
.brand small{display:block;color:var(--mute);font-size:11.5px;line-height:1.2}
.nav{display:flex;flex-direction:column;gap:2px}
.nav button{display:flex;align-items:center;gap:11px;padding:9px 10px;border:0;background:none;border-radius:10px;cursor:pointer;font-weight:500;color:var(--mute);text-align:left}
.nav button svg{width:20px;height:20px;flex:none}
.nav button:hover{background:var(--panel2);color:var(--ink)}
.nav button[aria-current="page"]{background:var(--accent-soft);color:var(--accent);font-weight:600}
.nav .badge{margin-left:auto;font:600 11px var(--f-mono);background:var(--crit);color:#fff;border-radius:99px;padding:1px 7px}
.side .foot{margin-top:auto;font-size:12px;color:var(--mute);padding:0 8px;display:flex;flex-direction:column;gap:4px}
.side .foot span{font-family:var(--f-mono)}
/* ---- phần chính ---- */
main{min-width:0;padding-block:22px 40px;padding-left:max(24px,env(safe-area-inset-left,0px));padding-right:max(24px,env(safe-area-inset-right,0px))}
.top{display:flex;align-items:flex-end;justify-content:space-between;gap:12px;flex-wrap:wrap;margin-bottom:18px}
.top h1{font-family:var(--f-disp);font-size:28px;line-height:1.1;margin:0;text-wrap:balance}
.top .sub{color:var(--mute);margin-top:4px}
.chips{display:flex;gap:6px;flex-wrap:wrap}
.chip{display:inline-flex;align-items:center;gap:6px;border:1px solid var(--line);background:var(--panel);border-radius:99px;padding:4px 10px;font-size:12.5px;font-weight:500;white-space:nowrap}
.dot{width:8px;height:8px;border-radius:50%;background:var(--good);flex:none}
.dot.warn{background:var(--warn)}.dot.crit{background:var(--crit)}.dot.off{background:var(--mute)}
.mono{font-family:var(--f-mono)}
.banner{display:flex;align-items:center;gap:12px;flex-wrap:wrap;border-radius:14px;padding:12px 16px;margin-bottom:16px;background:var(--crit-soft);color:var(--crit);border:1px solid color-mix(in srgb,var(--crit) 35%,transparent)}
.banner b{font-size:15px}.banner span{color:var(--ink);flex:1;min-width:200px}
.grid{display:grid;grid-template-columns:repeat(12,minmax(0,1fr));gap:16px}
.card{background:var(--panel);border:1px solid var(--line);border-radius:16px;padding:16px 18px;min-width:0}
.card h2{font-family:var(--f-disp);font-size:17px;margin:0;display:flex;align-items:center;gap:8px}
.card .hd{display:flex;align-items:center;justify-content:space-between;gap:10px;margin-bottom:12px;flex-wrap:wrap}
.card .note{color:var(--mute);font-size:12.5px}
.s12{grid-column:span 12}.s8{grid-column:span 8}.s7{grid-column:span 7}.s6{grid-column:span 6}.s5{grid-column:span 5}.s4{grid-column:span 4}
@media (max-width:1180px){.s4{grid-column:span 6}.s5,.s7{grid-column:span 12}.s8{grid-column:span 12}}
@media (max-width:860px){.s4,.s6{grid-column:span 12}}
.pill{display:inline-flex;align-items:center;gap:5px;font-size:12px;font-weight:600;border-radius:99px;padding:2px 9px;background:var(--panel2);color:var(--mute);white-space:nowrap}
.pill.good{background:var(--good-soft);color:var(--good)}.pill.warn{background:var(--warn-soft);color:var(--warn)}.pill.crit{background:var(--crit-soft);color:var(--crit)}.pill.acc{background:var(--accent-soft);color:var(--accent)}
.btn{display:inline-flex;align-items:center;justify-content:center;gap:7px;border:1px solid var(--line);background:var(--panel2);border-radius:11px;padding:9px 14px;font-weight:600;cursor:pointer;min-height:40px}
.btn:hover{border-color:var(--accent)}
.btn.pri{background:var(--accent);border-color:var(--accent);color:var(--accent-ink)}
.btn.danger{color:var(--crit)}
.btn[disabled]{opacity:.45;cursor:not-allowed}
.btn svg{width:18px;height:18px}
.row{display:flex;gap:8px;flex-wrap:wrap}
/* ---- cửa + robot ---- */
.doorhero{display:grid;grid-template-columns:minmax(0,1fr);gap:14px}
.oled{background:var(--bezel);border-radius:14px;padding:12px;box-shadow:inset 0 0 0 1px rgba(255,255,255,.04)}
.oled canvas{display:block;width:100%;height:auto;image-rendering:pixelated;border-radius:4px;background:#03070a;aspect-ratio:2/1}
.doorstat{display:flex;align-items:center;justify-content:space-between;gap:10px;flex-wrap:wrap}
.doorstat .big{font-family:var(--f-disp);font-size:24px;line-height:1.1}
.doorstat .big small{display:block;font-family:var(--f-body);font-size:13px;color:var(--mute);font-weight:400}
.acts{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:8px}
@media (max-width:420px){.acts{grid-template-columns:minmax(0,1fr) minmax(0,1fr)}.acts .btn:first-child{grid-column:span 2}}
/* ---- cảm biến ---- */
.sens{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:10px}
@media (max-width:560px){.sens{grid-template-columns:repeat(2,minmax(0,1fr))}}
.tile{border:1px solid var(--line);border-radius:13px;padding:11px 12px;background:var(--panel2);display:flex;flex-direction:column;gap:2px;min-width:0}
.tile .lb{display:flex;align-items:center;gap:6px;color:var(--mute);font-size:12.5px;font-weight:500}
.tile .lb svg{width:16px;height:16px}
.tile .v{font-family:var(--f-disp);font-size:22px;line-height:1.15;font-variant-numeric:tabular-nums}
.tile .w{font-size:12.5px;font-weight:600}
.tile.bad{background:var(--crit-soft);border-color:color-mix(in srgb,var(--crit) 40%,transparent)}.tile.bad .w{color:var(--crit)}
.tile.ok .w{color:var(--good)}.tile.mid .w{color:var(--warn)}
.modebar{display:flex;align-items:center;gap:8px;flex-wrap:wrap}
/* ---- đèn ---- */
.ringwrap{display:flex;align-items:center;gap:16px}
.ring{width:84px;height:84px;flex:none}
.seg{display:inline-grid;grid-auto-flow:column;grid-auto-columns:1fr;border:1px solid var(--line);border-radius:10px;padding:3px;background:var(--panel2);gap:2px}
.seg button{border:0;background:none;border-radius:8px;padding:6px 10px;font-weight:600;font-size:12.5px;cursor:pointer;color:var(--mute);white-space:nowrap}
.seg button[aria-pressed="true"]{background:var(--panel);color:var(--accent);box-shadow:0 1px 2px rgba(0,0,0,.12)}
.lrow{display:flex;align-items:center;justify-content:space-between;gap:10px;padding:10px 0;border-top:1px solid var(--line);flex-wrap:wrap}
.lrow:first-of-type{border-top:0}
.lrow b{display:block}.lrow small{color:var(--mute)}
/* ---- biểu đồ ---- */
.chart #tChart{display:block;width:100%;height:auto}
.chart .axis{fill:var(--mute);font:11px var(--f-mono)}
.kpis{display:flex;gap:18px;margin-bottom:8px}
.kpis div{display:flex;flex-direction:column}.kpis span{color:var(--mute);font-size:12px}.kpis b{font-family:var(--f-disp);font-size:20px;font-variant-numeric:tabular-nums}
/* ---- LCD ---- */
.lcd{background:#1f6b2f;border-radius:10px;padding:9px;box-shadow:inset 0 0 0 2px #17521f}
.lcd canvas{display:block;width:100%;height:auto;border-radius:3px;image-rendering:pixelated}
/* ---- hoạt động ---- */
.feed{list-style:none;margin:0;padding:0;display:flex;flex-direction:column}
.feed li{display:grid;grid-template-columns:34px minmax(0,1fr) auto;gap:10px;align-items:center;padding:9px 0;border-top:1px solid var(--line)}
.feed li:first-child{border-top:0}
.feed .ic{width:34px;height:34px;border-radius:10px;display:grid;place-items:center;background:var(--panel2);font-size:16px}
.feed .ic.good{background:var(--good-soft)}.feed .ic.crit{background:var(--crit-soft)}.feed .ic.warn{background:var(--warn-soft)}.feed .ic.acc{background:var(--accent-soft)}
.feed .t{font-weight:500;overflow-wrap:anywhere}.feed .t small{display:block;color:var(--mute);font-weight:400}
.feed time{font:12px var(--f-mono);color:var(--mute);white-space:nowrap}
.filters{display:flex;gap:6px;flex-wrap:wrap}
.filters button{border:1px solid var(--line);background:var(--panel2);border-radius:99px;padding:5px 12px;font-size:12.5px;font-weight:600;cursor:pointer;color:var(--mute)}
.filters button[aria-pressed="true"]{background:var(--accent);border-color:var(--accent);color:var(--accent-ink)}
/* ---- thẻ ---- */
.cards{list-style:none;margin:0;padding:0}
.cards li{display:grid;grid-template-columns:40px minmax(0,1fr) auto;gap:12px;align-items:center;padding:12px 0;border-top:1px solid var(--line)}
.cards li:first-child{border-top:0}
.cards .av{width:40px;height:40px;border-radius:12px;background:var(--accent-soft);color:var(--accent);display:grid;place-items:center;font-family:var(--f-disp);font-weight:700;font-size:17px}
.cards .av.own{background:var(--warn-soft);color:var(--warn)}
.cards b{display:block}.cards small{color:var(--mute);font-family:var(--f-mono);font-size:12px}
.cards .ops{display:flex;gap:6px}
.cards input{font:inherit;border:1px solid var(--accent);border-radius:9px;padding:6px 9px;background:var(--panel);color:var(--ink);width:100%;max-width:220px}
.grantbox{border:1px dashed var(--accent);border-radius:14px;padding:14px;background:var(--accent-soft);display:flex;align-items:center;gap:14px;flex-wrap:wrap}
.grantbox .cd{font-family:var(--f-disp);font-size:30px;color:var(--accent);font-variant-numeric:tabular-nums;min-width:56px}
.grantbox p{margin:0;flex:1;min-width:200px}
.bar{height:6px;border-radius:99px;background:var(--line);overflow:hidden;margin-top:6px}.bar i{display:block;height:100%;background:var(--accent);border-radius:99px;transition:width .3s}
/* ---- chi tiết cảm biến ---- */
.slist{list-style:none;margin:0;padding:0}
.slist li{display:grid;grid-template-columns:38px minmax(0,1fr) auto;gap:12px;align-items:center;padding:12px 0;border-top:1px solid var(--line)}
.slist li:first-child{border-top:0}
.slist .ic{width:38px;height:38px;border-radius:11px;background:var(--panel2);display:grid;place-items:center}
.slist .ic svg{width:20px;height:20px}
.slist b{display:block}.slist small{color:var(--mute)}
.slist .val{text-align:right}.slist .val b{font-family:var(--f-disp);font-size:18px;font-variant-numeric:tabular-nums}
.gauge{display:block;width:min(260px,100%);height:6px;border-radius:99px;background:linear-gradient(90deg,#5aa7e8,#5cc58a 45%,#e9b44c 70%,#e5654f);position:relative;margin-top:6px;max-width:260px}
.gauge i{position:absolute;top:50%;width:12px;height:12px;border-radius:50%;background:var(--panel);border:3px solid var(--ink);transform:translate(-50%,-50%)}
/* ---- thông báo ---- */
.toast{position:fixed;left:50%;bottom:calc(24px + env(safe-area-inset-bottom,0px));transform:translateX(-50%) translateY(20px);background:var(--ink);color:var(--bg);border-radius:12px;padding:10px 16px;font-weight:600;opacity:0;transition:.25s;pointer-events:none;z-index:50;max-width:calc(100vw - 32px)}
.toast.on{opacity:1;transform:translateX(-50%) translateY(0)}
.confirm{display:flex;gap:8px;align-items:center;flex-wrap:wrap;margin-top:10px;padding:10px 12px;border-radius:12px;background:var(--warn-soft)}
.confirm span{flex:1;min-width:160px;font-weight:500}
.preview{margin-top:26px;color:var(--mute);font-size:12px;text-align:center}
section[hidden]{display:none!important}
/* ---- điện thoại: thanh tab dưới ---- */
.tabbar{display:none}
@media (max-width:860px){
  .app{grid-template-columns:minmax(0,1fr)}
  .side{display:none}
  main{padding-block:16px 96px;padding-left:max(16px,env(safe-area-inset-left,0px));padding-right:max(16px,env(safe-area-inset-right,0px))}
  .top h1{font-size:24px}
  .tabbar{display:grid;grid-template-columns:repeat(6,minmax(0,1fr));position:fixed;left:0;right:0;bottom:0;z-index:40;background:var(--panel);border-top:1px solid var(--line);
    padding:6px max(6px,env(safe-area-inset-right,0px)) calc(6px + env(safe-area-inset-bottom,0px)) max(6px,env(safe-area-inset-left,0px))}
  .tabbar button{border:0;background:none;display:flex;flex-direction:column;align-items:center;gap:2px;font-size:10.5px;white-space:nowrap;font-weight:600;color:var(--mute);padding:4px 0;cursor:pointer;position:relative}
  .tabbar svg{width:22px;height:22px}
  .tabbar button[aria-current="page"]{color:var(--accent)}
  .tabbar .badge{position:absolute;top:0;left:56%;font:600 10px var(--f-mono);background:var(--crit);color:#fff;border-radius:99px;padding:0 5px}
  .card{padding:14px}
  .toast{bottom:calc(84px + env(safe-area-inset-bottom,0px))}
}
@media (prefers-reduced-motion:reduce){*{transition:none!important}}
/* ---- ngôi nhà 2.5D ---- */
.crumbs{display:flex;flex-wrap:wrap;gap:6px;align-items:center;font-size:13px;color:var(--mute);margin:0 0 10px}
.crumbs button{background:none;border:0;padding:4px 6px;border-radius:7px;color:var(--mute);cursor:pointer;font-weight:500}
.crumbs button[aria-current="step"]{color:var(--ink);font-weight:700;background:var(--panel)}
.crumbs .sep{opacity:.5}
.stage{position:relative;border-radius:18px;overflow:hidden;border:1px solid var(--line);aspect-ratio:16/10;min-height:460px;background:linear-gradient(180deg,var(--skyA),var(--skyB));transition:background 1.2s}
.stage{--skyA:#b4835a;--skyB:#ecd5b2}
.stage.night{--skyA:#110b0b;--skyB:#3a2222}
.stage.inside{--skyA:#e9e4da;--skyB:#d8d0c2}
.stage.inside.night{--skyA:#1a1d24;--skyB:#2a2e38}
.stage.door{--skyA:#dfe5ea;--skyB:#cfd7de}
:root[data-theme="dark"] .stage.door{--skyA:#1c2630;--skyB:#141c24}
@media (prefers-color-scheme:dark){:root:not([data-theme="light"]) .stage.door{--skyA:#1c2630;--skyB:#141c24}}
@media (max-width:640px){.stage{aspect-ratio:auto;height:540px;min-height:0}}
.sc{position:absolute;inset:0;visibility:visible;transition:opacity .45s ease,transform .7s cubic-bezier(.2,.7,.2,1),visibility 0s}
.sc[data-on="0"]{opacity:0;visibility:hidden;pointer-events:none;transition:opacity .45s ease,transform .7s cubic-bezier(.2,.7,.2,1),visibility 0s linear .7s}
.sc>svg{width:100%;height:100%;display:block}
.stars{position:absolute;inset:0;pointer-events:none;opacity:0;transition:opacity 1.2s;
  background-image:radial-gradient(1.5px 1.5px at 12% 18%,#fff 50%,transparent 51%),radial-gradient(1px 1px at 28% 9%,#fff 50%,transparent 51%),radial-gradient(1.5px 1.5px at 47% 14%,#fff 50%,transparent 51%),radial-gradient(1px 1px at 63% 6%,#fff 50%,transparent 51%),radial-gradient(1.5px 1.5px at 81% 16%,#fff 50%,transparent 51%),radial-gradient(1px 1px at 91% 8%,#fff 50%,transparent 51%)}
.stage.night:not(.inside):not(.door) .stars{opacity:.45}
.fogA{animation:fog 16s ease-in-out infinite alternate}.fogB{animation:fog 22s ease-in-out infinite alternate-reverse}
@keyframes fog{from{transform:translateX(-50px)}to{transform:translateX(50px)}}
#doorsvg.can{cursor:pointer}#doorsvg.can:hover #leafHl{opacity:1}
.stagebar{position:absolute;left:0;right:0;bottom:0;display:flex;flex-wrap:wrap;gap:8px;justify-content:flex-end;align-items:center;padding:12px 14px;pointer-events:none}
.stagebar>*{pointer-events:auto}
.stagebar .btn{box-shadow:0 2px 8px rgba(0,0,0,.18)}
.btn.go{background:var(--accent);color:var(--accent-ink);border-color:transparent;font-weight:700}
.btn.go:disabled{background:var(--panel2);color:var(--mute)}
.hintpill{background:rgba(255,255,255,.92);color:#15202b;border-radius:999px;padding:7px 12px;font-size:12.5px;box-shadow:0 2px 8px rgba(0,0,0,.15)}
.guide{position:absolute;left:14px;top:14px;display:flex;gap:10px;align-items:flex-start;pointer-events:none;max-width:calc(100% - 28px)}
.guide .face{width:112px;flex:none;background:#020406;border:3px solid #2a3340;border-radius:10px;padding:4px}
.guide canvas{display:block;width:100%;aspect-ratio:2/1;image-rendering:pixelated}
.bubble{background:#fff;color:#15202b;border-radius:12px;padding:8px 12px;font-size:13px;line-height:1.4;box-shadow:0 2px 10px rgba(0,0,0,.18);position:relative;max-width:300px;margin-top:6px}
.bubble:before{content:"";position:absolute;left:-6px;top:14px;border:6px solid transparent;border-right-color:#fff;border-left:0}
.stage.door .guide{display:none}
@media (max-width:640px){.guide .face{width:84px}.bubble{font-size:12px;max-width:200px}}
.hs{cursor:pointer}
.hs .hl{fill:none;stroke:#fff;stroke-width:2.5;opacity:0;transition:opacity .15s}
.hs:hover .hl,.hs:focus-visible .hl,.hs.sel .hl{opacity:1}
.hs:focus{outline:none}
.tag{pointer-events:none;transition:opacity .2s}
.labels-off .tag.opt{opacity:0}
.pin{animation:bob 1.6s ease-in-out infinite}
@keyframes bob{0%,100%{transform:translateY(0)}50%{transform:translateY(-5px)}}
.rain line{stroke:rgba(210,230,255,.75);stroke-width:1.6;stroke-linecap:round}
.rain{animation:rain .5s linear infinite}
@keyframes rain{from{transform:translate(0,0)}to{transform:translate(-14px,40px)}}
.smoke circle{fill:#9aa3ab;opacity:0;animation:smoke 2.4s ease-out infinite}
.smoke circle:nth-child(2){animation-delay:.8s}.smoke circle:nth-child(3){animation-delay:1.6s}
@keyframes smoke{0%{opacity:.75;transform:translate(0,0) scale(.6)}100%{opacity:0;transform:translate(18px,-70px) scale(2.2)}}
.flame{animation:flick .18s steps(2) infinite}
@keyframes flick{50%{transform:translateY(-2px) scaleY(1.08)}}
.buzz circle{fill:none;stroke:#ffd166;stroke-width:1.5;opacity:0}
.buzz.on circle{animation:ring .5s ease-out infinite}
.buzz.on circle:nth-child(2){animation-delay:.25s}
@keyframes ring{0%{opacity:.9;r:3}100%{opacity:0;r:13}}
.zb{will-change:transform}
.doorscene{padding:20px 20px 74px;height:100%;overflow:auto;display:grid;place-items:center}
.dsbox{display:grid;grid-template-columns:minmax(0,260px) 220px;gap:22px;align-items:center;background:var(--panel);border:1px solid var(--line);border-radius:16px;padding:18px;max-width:100%}
.dsbox #doorsvg{width:100%;height:auto;display:block}
.dsside{display:flex;flex-direction:column;gap:8px;min-width:0}
.dsst{font-family:var(--f-disp);font-size:18px}
.rcbox{display:flex;align-items:center;justify-content:center;gap:4px;height:46px;border-radius:10px;background:#1a5aa0;color:#fff;font:600 12px var(--f-mono);position:relative}
.rcbox span{width:10px;height:10px;border-radius:50%;border:2px solid rgba(255,255,255,.5)}
.rcbox.on span{animation:rc 1s ease-out infinite}
@keyframes rc{0%{box-shadow:0 0 0 0 rgba(143,240,248,.8)}100%{box-shadow:0 0 0 10px rgba(143,240,248,0)}}
@media (max-width:640px){.dsbox{grid-template-columns:minmax(0,1fr);gap:12px;padding:12px}.dsbox #doorsvg{max-width:200px;margin:0 auto}}
.infobox{position:absolute;right:14px;top:14px;width:min(320px,calc(100% - 28px));background:var(--panel);color:var(--ink);border:1px solid var(--line);border-radius:12px;padding:14px 14px 12px;box-shadow:0 8px 28px rgba(0,0,0,.22);z-index:3;max-height:calc(100% - 90px);overflow:auto}
.infobox h3{font-size:15px;margin:0 0 2px;padding-right:28px}
.infobox .x{position:absolute;right:8px;top:8px;width:28px;height:28px;border-radius:8px;border:0;background:var(--panel2);color:var(--ink);cursor:pointer;font-size:16px;line-height:1}
.infobox .sub{font-size:12px;color:var(--mute);margin-bottom:10px}
.infobox .big{font-family:var(--f-disp);font-size:24px;font-weight:700;font-variant-numeric:tabular-nums;line-height:1.15}
.infobox p{margin:8px 0 0;font-size:13px}
.infobox .kv{display:grid;grid-template-columns:auto minmax(0,1fr);gap:5px 12px;font-size:13px;margin:10px 0 0}
.infobox .kv dt{color:var(--mute)}.infobox .kv dd{margin:0}
.spark{display:block;width:100%;height:46px;margin-top:8px}
.small{font-size:12px;color:var(--mute)}
@media (prefers-reduced-motion:reduce){.fogA,.fogB,.rain,.smoke circle,.flame,.pin,.buzz.on circle,.rcbox.on span{animation:none}.sc,.stage{transition:none}}
/* trong nhà: cảnh bên trái, cột phải riêng cho LCD + thông tin linh kiện, không chồng lên nhau */
#sc-in{display:grid;grid-template-columns:minmax(0,1fr)}
#sc-in>svg{grid-column:1;grid-row:1;width:100%;height:100%;min-height:0;padding:6px 0 58px}
.inrail{position:absolute;right:14px;top:14px;width:clamp(230px,27%,330px);max-height:calc(100% - 90px);display:flex;flex-direction:column;gap:10px;overflow:auto;z-index:2}
@media (min-width:641px){.stage.inside{aspect-ratio:16/11}}
.exitglow{animation:exitb .8s steps(2) infinite}@keyframes exitb{50%{opacity:.35}}
.lcdpanel{background:var(--panel);border:1px solid var(--line);border-radius:14px;padding:10px;box-shadow:0 6px 18px rgba(0,0,0,.14)}
.lcdpanel .cap{display:flex;justify-content:space-between;gap:8px;margin-top:7px;font-size:12px;color:var(--mute)}
.lcdpanel .cap span:last-child{color:var(--ink);font-weight:600;text-align:right}
.kp{margin-top:10px}.kpk{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:6px}
.kpk button{position:relative;overflow:hidden;appearance:none;border:0;border-radius:9px;background:#2a3037;color:#eef3f6;padding:6px 4px 7px;cursor:pointer;box-shadow:0 3px 0 #14181c;touch-action:none;user-select:none;-webkit-user-select:none;font:inherit;line-height:1.15}
.kpk button b{display:block;font:700 15px/1.1 ui-monospace,Menlo,Consolas,monospace}
.kpk button span{display:block;font-size:12px;font-weight:600;margin-top:2px}
.kpk button i{display:block;font-style:normal;font-size:10px;color:#a9b4be;margin-top:1px}
.kpk button::after{content:"";position:absolute;left:0;bottom:0;height:3px;width:calc(var(--p,0)*100%);background:#35c3d2}
.kpk button.on{transform:translateY(2px);box-shadow:0 1px 0 #14181c;background:#3a424b}.kpk button.done{background:#0b6f7b}
.kpk button:focus-visible{outline:3px solid var(--accent,#0b8a99);outline-offset:2px}
.kph{display:block;font-size:11px;color:var(--mute);margin-top:6px;text-align:center}
.inrail .infobox{position:relative;right:auto;top:auto;bottom:auto;width:auto;max-height:none;box-shadow:0 6px 18px rgba(0,0,0,.14)}
.stage.inside .guide{max-width:calc(70% - 60px)}
.stage.inside .bubble{max-width:260px}
@media (max-width:640px){
  #sc-in{grid-template-columns:minmax(0,1fr);grid-template-rows:auto minmax(0,1fr)}
  #sc-in>svg{grid-column:1;grid-row:2;padding:0 0 64px}
  .inrail{position:static;width:auto;max-height:none;grid-column:1;grid-row:1;padding:10px 10px 0;overflow:visible}
  .lcdpanel{padding:8px}
  .inrail{gap:8px}
  .inrail .infobox{padding:10px 12px}
  .stage.inside{height:680px}
  .stage.inside .guide{display:none}
}

</style>

<svg width="0" height="0" style="position:absolute" aria-hidden="true">
  <symbol id="i-home" viewBox="0 0 24 24"><path d="M3 11 12 4l9 7v9H3z" fill="none" stroke="currentColor" stroke-width="2" stroke-linejoin="round"/><path d="M10 20v-5h4v5" fill="none" stroke="currentColor" stroke-width="2"/></symbol>
  <symbol id="i-iso" viewBox="0 0 24 24"><path d="M12 3 3 8v8l9 5 9-5V8z" fill="none" stroke="currentColor" stroke-width="2" stroke-linejoin="round"/><path d="M3 8l9 5 9-5M12 13v8" fill="none" stroke="currentColor" stroke-width="2" stroke-linejoin="round"/></symbol>
  <symbol id="i-grid" viewBox="0 0 24 24"><rect x="3" y="3" width="7.5" height="7.5" rx="2" fill="none" stroke="currentColor" stroke-width="2"/><rect x="13.5" y="3" width="7.5" height="7.5" rx="2" fill="none" stroke="currentColor" stroke-width="2"/><rect x="3" y="13.5" width="7.5" height="7.5" rx="2" fill="none" stroke="currentColor" stroke-width="2"/><rect x="13.5" y="13.5" width="7.5" height="7.5" rx="2" fill="none" stroke="currentColor" stroke-width="2"/></symbol>
  <symbol id="i-door" viewBox="0 0 24 24"><rect x="6" y="3" width="12" height="18" rx="1.5" fill="none" stroke="currentColor" stroke-width="2"/><circle cx="14.5" cy="12.5" r="1.3" fill="currentColor"/></symbol>
  <symbol id="i-card" viewBox="0 0 24 24"><rect x="3" y="5" width="18" height="14" rx="2.5" fill="none" stroke="currentColor" stroke-width="2"/><rect x="6" y="9" width="5" height="4" rx="1" fill="currentColor"/><path d="M14 10h4M14 14h3" stroke="currentColor" stroke-width="2" stroke-linecap="round"/></symbol>
  <symbol id="i-list" viewBox="0 0 24 24"><path d="M8 6h13M8 12h13M8 18h13" stroke="currentColor" stroke-width="2" stroke-linecap="round"/><circle cx="4" cy="6" r="1.4" fill="currentColor"/><circle cx="4" cy="12" r="1.4" fill="currentColor"/><circle cx="4" cy="18" r="1.4" fill="currentColor"/></symbol>
  <symbol id="i-unlock" viewBox="0 0 24 24"><rect x="5" y="11" width="14" height="10" rx="2" fill="none" stroke="currentColor" stroke-width="2"/><path d="M8 11V7a4 4 0 0 1 7.8-1.2" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"/></symbol>
  <symbol id="i-lock" viewBox="0 0 24 24"><rect x="5" y="11" width="14" height="10" rx="2" fill="none" stroke="currentColor" stroke-width="2"/><path d="M8 11V7a4 4 0 0 1 8 0v4" fill="none" stroke="currentColor" stroke-width="2"/></symbol>
  <symbol id="i-plus" viewBox="0 0 24 24"><path d="M12 5v14M5 12h14" stroke="currentColor" stroke-width="2.2" stroke-linecap="round"/></symbol>
  <symbol id="i-therm" viewBox="0 0 24 24"><path d="M10 4a2 2 0 0 1 4 0v10a4 4 0 1 1-4 0z" fill="none" stroke="currentColor" stroke-width="2"/><circle cx="12" cy="17" r="1.6" fill="currentColor"/></symbol>
  <symbol id="i-drop" viewBox="0 0 24 24"><path d="M12 3s6 7 6 11a6 6 0 0 1-12 0c0-4 6-11 6-11z" fill="none" stroke="currentColor" stroke-width="2" stroke-linejoin="round"/></symbol>
  <symbol id="i-sun" viewBox="0 0 24 24"><circle cx="12" cy="12" r="4" fill="none" stroke="currentColor" stroke-width="2"/><path d="M12 2v2M12 20v2M2 12h2M20 12h2M4.9 4.9l1.4 1.4M17.7 17.7l1.4 1.4M4.9 19.1l1.4-1.4M17.7 6.3l1.4-1.4" stroke="currentColor" stroke-width="2" stroke-linecap="round"/></symbol>
  <symbol id="i-moon" viewBox="0 0 24 24"><path d="M20 14.5A8 8 0 1 1 9.5 4a6.5 6.5 0 0 0 10.5 10.5z" fill="none" stroke="currentColor" stroke-width="2" stroke-linejoin="round"/></symbol>
  <symbol id="i-rain" viewBox="0 0 24 24"><path d="M7 15a4 4 0 0 1 .5-8A5 5 0 0 1 17 8a3.5 3.5 0 0 1 0 7z" fill="none" stroke="currentColor" stroke-width="2" stroke-linejoin="round"/><path d="M9 18l-1 2M13 18l-1 2M17 18l-1 2" stroke="currentColor" stroke-width="2" stroke-linecap="round"/></symbol>
  <symbol id="i-fire" viewBox="0 0 24 24"><path d="M12 3c1 4 5 5 5 10a5 5 0 0 1-10 0c0-2 1-3.5 2-4.5 0 2 1 3 2 3-1-3 0-6 1-8.5z" fill="none" stroke="currentColor" stroke-width="2" stroke-linejoin="round"/></symbol>
  <symbol id="i-sound" viewBox="0 0 24 24"><path d="M4 10v4M8 7v10M12 4v16M16 8v8M20 11v2" stroke="currentColor" stroke-width="2" stroke-linecap="round"/></symbol>
  <symbol id="i-bulb" viewBox="0 0 24 24"><path d="M9 17h6M10 21h4M12 3a6 6 0 0 0-3.5 10.9c.8.6 1 1.3 1 2.1h5c0-.8.2-1.5 1-2.1A6 6 0 0 0 12 3z" fill="none" stroke="currentColor" stroke-width="2" stroke-linejoin="round"/></symbol>
  <symbol id="i-walk" viewBox="0 0 24 24"><path d="M4 12h12M12 7l5 5-5 5" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/><path d="M20 4v16" stroke="currentColor" stroke-width="2" stroke-linecap="round"/></symbol>
  <symbol id="i-bell" viewBox="0 0 24 24"><path d="M6 16V11a6 6 0 0 1 12 0v5l2 2H4z" fill="none" stroke="currentColor" stroke-width="2" stroke-linejoin="round"/><path d="M10 21h4" stroke="currentColor" stroke-width="2" stroke-linecap="round"/></symbol>
</svg>

<div class="app">
  <aside class="side" aria-label="Điều hướng">
    <div class="brand"><div class="logo"><svg width="22" height="12" viewBox="0 0 22 12" aria-hidden="true"><rect x="1" y="1" width="6" height="9" rx="2.6" fill="#8ff0f8"/><rect x="15" y="1" width="6" height="9" rx="2.6" fill="#8ff0f8"/><path d="M9 9.5q2 2 4 0" stroke="#8ff0f8" stroke-width="1.6" fill="none" stroke-linecap="round"/></svg></div>
      <div><b>Cerberus</b><small>cerberus.local</small></div></div>
    <nav class="nav" id="nav"></nav>
    <div class="foot"><div>Cửa · ESP32 <span id="fIp">192.168.1.42</span></div><div>Nhà · Arduino Uno qua dây Serial</div><div>Bật được <span id="fUp">—</span></div></div>
  </aside>

  <main>
    <div class="top">
      <div><h1 id="title">Tổng quan</h1><div class="sub" id="greet">—</div></div>
      <div class="chips">
        <span class="chip"><i class="dot" id="dWifi"></i><span id="tWifi">WiFi tốt</span></span>
        <span class="chip"><i class="dot" id="dLink"></i><span id="tLink">Nhà đã nối</span></span>
        <span class="chip"><i class="dot" id="dTg"></i>Telegram</span>
        <span class="chip mono" id="clock">--:--</span>
      </div>
    </div>
    <div class="banner" id="alarmBar" hidden><b id="alarmT">🚨 Báo động</b><span id="alarmS"></span><button class="btn danger" type="button" data-act="silence">Tắt báo động</button></div>

    <!-- ================= TỔNG QUAN ================= -->
    <section data-page="tong-quan">
      <div class="grid">
        <div class="card s5">
          <div class="hd"><h2><svg width="18" height="18"><use href="#i-door"/></svg>Cửa trước</h2><span class="pill" id="doorPill">—</span></div>
          <div class="doorhero">
            <div class="oled"><canvas id="oled" width="128" height="64" aria-label="Màn OLED của robot ở cửa"></canvas></div>
            <div class="doorstat"><div class="big" id="doorBig">Đang đóng<small id="doorSmall">—</small></div><span class="pill" id="sysPill"></span></div>
            <div class="acts">
              <button class="btn pri" type="button" data-act="open"><svg><use href="#i-unlock"/></svg>Mở cửa</button>
              <button class="btn" type="button" data-act="grant" id="bGrant"><svg><use href="#i-plus"/></svg>Thêm thẻ</button>
              <button class="btn" type="button" data-act="syslock" id="bSys"><svg><use href="#i-lock"/></svg>Khóa thẻ</button>
            </div>
            <div class="confirm" id="openAsk" hidden><span>Mở cửa trước? Cửa tự đóng sau 5 giây.</span><button class="btn pri" type="button" data-act="openYes">Mở</button><button class="btn" type="button" data-act="openNo">Thôi</button></div>
          </div>
        </div>
        <div class="card s7">
          <div class="hd"><h2><svg width="18" height="18"><use href="#i-home"/></svg>Trong nhà</h2><div class="modebar"><span class="pill acc" id="modePill">Ở nhà</span><span class="note" id="homeSum">—</span></div></div>
          <div class="sens" id="sensTiles"></div>
          <div class="row" style="margin-top:12px">
            <button class="btn" type="button" data-hmode="away"><svg><use href="#i-walk"/></svg>Vắng nhà</button>
            <button class="btn" type="button" data-hmode="sleep"><svg><use href="#i-moon"/></svg>Đi ngủ</button>
            <button class="btn" type="button" data-hmode="home" hidden><svg><use href="#i-home"/></svg>Về nhà</button>
          </div>
        </div>
        <div class="card s4">
          <div class="hd"><h2><svg width="18" height="18"><use href="#i-bulb"/></svg>Đèn</h2><span class="note" id="ringFx">Đèn thường</span></div>
          <div class="ringwrap"><svg class="ring" id="ring" viewBox="0 0 84 84" role="img" aria-label="Vòng đèn NeoPixel 12 bóng"></svg>
            <div style="min-width:0"><b id="ringTxt">Phòng khách đang tắt</b><div class="note" id="ringNote">Tự bật khi trời tối</div></div></div>
          <div class="lrow"><div><b>Phòng khách</b><small>Đèn trần NeoPixel</small></div><div class="seg" data-light="ring"><button type="button" data-v="0">Tự động</button><button type="button" data-v="1">Bật</button><button type="button" data-v="2">Tắt</button></div></div>
          <div class="lrow"><div><b>Hành lang</b><small id="hallTxt">Đang tắt</small></div><div class="seg" data-light="hall"><button type="button" data-v="0">Tự động</button><button type="button" data-v="1">Bật</button><button type="button" data-v="2">Tắt</button></div></div>
        </div>
        <div class="card s4 chart">
          <div class="hd"><h2><svg width="18" height="18"><use href="#i-therm"/></svg>Nhiệt độ 24 giờ</h2><span class="note">DHT11</span></div>
          <div class="kpis"><div><span>Bây giờ</span><b id="kNow">—</b></div><div><span>Thấp nhất</span><b id="kMin">—</b></div><div><span>Cao nhất</span><b id="kMax">—</b></div></div>
          <svg id="tChart" viewBox="0 0 320 130" role="img" aria-label="Biểu đồ nhiệt độ 24 giờ qua"></svg>
        </div>
        <div class="card s4">
          <div class="hd"><h2>Màn hình trong nhà</h2><span class="note" id="lcdPage">—</span></div>
          <div class="lcd"><canvas id="lcd" width="400" height="84" aria-label="Nội dung LCD1602 trong nhà"></canvas></div>
          <p class="note" style="margin:10px 0 0">Hình chép từ màn LCD thật, Arduino gửi sang ESP32 qua dây Serial.</p>
        </div>
        <div class="card s12">
          <div class="hd"><h2><svg width="18" height="18"><use href="#i-list"/></svg>Hoạt động gần đây</h2><button class="btn" type="button" data-go="nhat-ky">Xem tất cả</button></div>
          <ul class="feed" id="feedShort"></ul>
        </div>
      </div>
    </section>

    <!-- ================= NGÔI NHÀ (2.5D, chạy theo dữ liệu thật) ================= -->
    <section data-page="ngoi-nha" hidden>
      <nav class="crumbs" aria-label="Vị trí trong ngôi nhà">
        <button type="button" data-view="out" aria-current="step">🌳 Ngoài sân</button><span class="sep">›</span>
        <button type="button" data-view="door">🚪 Cửa</button><span class="sep">›</span>
        <button type="button" data-view="in">🏠 Trong nhà</button>
      </nav>
      <div class="stage" id="stage" aria-label="Ngôi nhà 2.5D">
        <div class="stars"></div>
        <div class="sc" id="sc-out" data-on="1">
          <svg id="outSvg" viewBox="-440 -300 900 720" preserveAspectRatio="xMidYMid meet" role="img" aria-label="Ngôi nhà nhìn từ ngoài"></svg>
          <div class="stagebar"><span class="hintpill">Bấm vào cửa để đến gần</span><button class="btn go" type="button" data-view="door">Đến cửa →</button></div>
        </div>
        <div class="sc" id="sc-door" data-on="0">
          <div class="doorscene">
            <div class="dsbox">
              <svg viewBox="0 0 300 300" role="button" tabindex="0" aria-label="Cửa ra vào: khi cửa mở, bấm để vào nhà" id="doorsvg">
                <rect x="0" y="0" width="300" height="300" rx="8" fill="var(--wall)"/>
                <rect x="70" y="40" width="150" height="240" fill="var(--inside)"/>
                <rect id="glow" x="70" y="40" width="150" height="240" fill="#ffcf7a" opacity="0"/>
                <g id="leaf"><rect x="70" y="40" width="150" height="240" fill="var(--door)"/>
                  <rect x="86" y="58" width="52" height="92" rx="3" fill="none" stroke="var(--door2)" stroke-width="4"/><rect x="152" y="58" width="52" height="92" rx="3" fill="none" stroke="var(--door2)" stroke-width="4"/>
                  <rect x="86" y="166" width="52" height="96" rx="3" fill="none" stroke="var(--door2)" stroke-width="4"/><rect x="152" y="166" width="52" height="96" rx="3" fill="none" stroke="var(--door2)" stroke-width="4"/>
                  <circle cx="206" cy="160" r="6" fill="#e8e1d2" stroke="var(--door2)" stroke-width="2"/></g>
                <rect x="66" y="36" width="158" height="248" fill="none" stroke="var(--frame)" stroke-width="8"/>
                <rect id="leafHl" x="70" y="40" width="150" height="240" fill="none" stroke="#0b8a99" stroke-width="5" opacity="0" style="transition:opacity .15s"/>
                <g id="enterTxt" opacity="0" style="transition:opacity .3s;pointer-events:none"><rect x="98" y="138" width="110" height="34" rx="17" fill="#0b8a99"/><text x="153" y="160" text-anchor="middle" font-size="14" font-weight="700" fill="#fff" font-family="system-ui,sans-serif">Vào nhà →</text></g>
                <rect x="30" y="52" width="26" height="34" rx="4" fill="var(--frame)"/><text x="43" y="73" font-size="9" text-anchor="middle" fill="var(--wall)" font-family="monospace">SG90</text>
                <line x1="56" y1="69" x2="66" y2="69" stroke="var(--frame)" stroke-width="3"/><rect x="0" y="284" width="300" height="16" fill="var(--frame)" opacity=".35"/>
              </svg>
              <div class="dsside">
                <div class="oled"><canvas id="oledScene" width="128" height="64" aria-label="Màn OLED ở cửa"></canvas></div>
                <div class="rcbox" id="rcBox"><span></span><span></span><b>RC522</b></div>
                <b class="dsst" id="dsSt">—</b><span class="note" id="dsSub">—</span>
                <button class="btn pri" type="button" data-act="openDirect"><svg><use href="#i-unlock"/></svg>Mở cửa</button>
              </div>
            </div>
          </div>
          <div class="stagebar"><button class="btn" type="button" data-view="out">← Ra sân</button><span class="hintpill" id="doorHint">Cửa mở thì bấm vào cánh cửa để vào nhà</span><button class="btn go" type="button" id="btnEnter" data-view="in" disabled>Vào nhà →</button></div>
        </div>
        <div class="sc" id="sc-in" data-on="0">
          <svg id="inSvg" viewBox="-236 -150 590 465" preserveAspectRatio="xMidYMid meet" role="img" aria-label="Bên trong ngôi nhà"></svg>
          <div class="inrail"><div class="lcdpanel" aria-label="Màn hình LCD trong nhà, phóng to"><div class="lcd"><canvas id="lcdScene" width="400" height="84" aria-label="Nội dung LCD1602 trong nhà"></canvas></div><div class="cap"><span>LCD1602 · trong nhà</span><span id="lcdScenePage">—</span></div></div>
          <div class="infobox" id="info" hidden></div></div>
          <div class="stagebar"><button class="btn" type="button" data-view="door">← Ra cửa</button><button class="btn" type="button" id="btnLabels" aria-pressed="true">Ẩn tên linh kiện</button><span class="hintpill">Bấm vào linh kiện để xem chi tiết</span></div>
        </div>
        <div class="guide"><div class="face"><canvas id="oledGuide" width="128" height="64" aria-hidden="true"></canvas></div><div class="bubble" id="bubble">Xin chào!</div></div>
      </div>
      <p class="note" style="margin:10px 2px 0">Mọi thứ trong cảnh chạy theo dữ liệu thật: trời tối theo cảm biến ánh sáng, cánh cửa quay theo servo, vòng đèn đúng màu NeoPixel, LCD chép từ màn thật.</p>
    </section>


    <!-- ================= CỬA ================= -->
    <section data-page="cua" hidden>
      <div class="grid">
        <div class="card s6">
          <div class="hd"><h2>Robot ở cửa</h2><span class="pill" id="doorPill2">—</span></div>
          <div class="oled"><canvas id="oledBig" width="128" height="64" aria-label="Màn OLED phóng to"></canvas></div>
          <div class="row" style="margin-top:12px">
            <button class="btn pri" type="button" data-act="open"><svg><use href="#i-unlock"/></svg>Mở cửa</button>
            <button class="btn danger" type="button" data-act="silence" id="bSilence">Tắt báo động</button>
          </div>
        </div>
        <div class="card s6">
          <div class="hd"><h2>Bảo vệ</h2></div>
          <div class="lrow"><div><b>Khóa thẻ thành viên</b><small>Bật thì chỉ thẻ chủ nhà và lệnh từ xa mở được cửa</small></div><div class="seg" id="segSys"><button type="button" data-v="0">Tắt</button><button type="button" data-v="1">Bật</button></div></div>
          <div class="lrow"><div><b>Quẹt sai liên tiếp</b><small>3 lần thì đầu đọc khóa 30 giây</small></div><b class="mono" id="fails">0 / 3</b></div>
          <div class="lrow"><div><b>Cảm biến rung</b><small>SW-420 ở cánh cửa</small></div><span class="pill good" id="vibPill">Yên</span></div>
          <div class="lrow"><div><b>Người trước cửa</b><small>SRF04</small></div><span class="mono" id="distTxt">—</span></div>
        </div>
        <div class="card s12">
          <div class="hd"><h2>Ra vào hôm nay</h2></div>
          <ul class="feed" id="feedDoor"></ul>
        </div>
      </div>
    </section>

    <!-- ================= TRONG NHÀ ================= -->
    <section data-page="trong-nha" hidden>
      <div class="grid">
        <div class="card s7">
          <div class="hd"><h2>Cảm biến</h2><span class="note" id="homeSum2">—</span></div>
          <ul class="slist" id="sensList"></ul>
        </div>
        <div class="card s5">
          <div class="hd"><h2>Chế độ nhà</h2></div>
          <div class="seg" id="segMode" style="width:100%"><button type="button" data-v="home">Ở nhà</button><button type="button" data-v="away">Vắng nhà</button><button type="button" data-v="sleep">Đi ngủ</button></div>
          <p class="note" id="modeNote" style="margin:10px 0 14px">—</p>
          <div class="lrow"><div><b>Báo thức</b><small>Bình minh trên đèn trần + còi</small></div><b class="mono">06:00</b></div>
          <div class="lrow"><div><b>Tự đi ngủ</b><small>Màn hình đồng hồ lớn, tắt đèn</small></div><b class="mono">23:00</b></div>
          <div class="lcd" style="margin-top:12px"><canvas id="lcd2" width="400" height="84" aria-label="Nội dung LCD1602 trong nhà"></canvas></div>
        </div>
      </div>
    </section>

    <!-- ================= THẺ ================= -->
    <section data-page="the" hidden>
      <div class="grid">
        <div class="card s12">
          <div class="hd"><h2>Thẻ được vào nhà</h2><span class="note" id="cardCount">—</span></div>
          <div class="grantbox" id="grantBox" hidden><div class="cd" id="grantCd">15</div><p><b>Đang chờ thẻ mới.</b> Quẹt thẻ cần thêm vào đầu đọc ở cửa. Quẹt thẻ thành viên là thu hồi.<span class="bar"><i id="grantBar" style="width:100%"></i></span></p><button class="btn" type="button" data-act="grant_stop">Dừng</button></div>
          <ul class="cards" id="cardList"></ul>
          <div class="row" style="margin-top:12px"><button class="btn pri" type="button" data-act="grant"><svg><use href="#i-plus"/></svg>Thêm thẻ mới</button></div>
        </div>
      </div>
    </section>

    <!-- ================= NHẬT KÝ ================= -->
    <section data-page="nhat-ky" hidden>
      <div class="card">
        <div class="hd"><h2>Nhật ký</h2><div class="filters" id="filters"><button type="button" data-f="all" aria-pressed="true">Tất cả</button><button type="button" data-f="door" aria-pressed="false">Cửa</button><button type="button" data-f="home" aria-pressed="false">Trong nhà</button><button type="button" data-f="warn" aria-pressed="false">Cảnh báo</button></div></div>
        <ul class="feed" id="feedAll"></ul>
      </div>
    </section>
  </main>
</div>
<nav class="tabbar" id="tabbar" aria-label="Điều hướng"></nav>
<div class="toast" id="toast" role="status"></div>

<script>
var $=function(s,r){return (r||document).querySelector(s)},$$=function(s,r){return Array.prototype.slice.call((r||document).querySelectorAll(s))};
function pad(n){return (n<10?'0':'')+n}
function secLeft(ms){return Math.max(0,Math.ceil((ms-Date.now())/1000))}
function ascii(s){return String(s||'').normalize('NFD').replace(/[̀-ͯ]/g,'').replace(/đ/g,'d').replace(/Đ/g,'D')}
function esc(s){return String(s).replace(/[&<>"]/g,function(c){return {'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;'}[c]})}
var S={mode:'IDLE',greet:'',fails:0,sysLock:false,until:0,grantUntil:0,dist:-1,cute:false,open:0,servo:0,door:false,lastOpen:0};
</script>
<script>
/* Biểu cảm ảnh (★): khóa = ngủ, thức dậy, mắt trái tim — bộ hình @khoi2mai, lấy từ ESP32 qua /api/faces (giống hệt màn OLED) */
var WAKE_MS=1180,BMP=null,BMI={};
(function(){if(location.protocol==='file:')return;
  fetch('/api/faces',{cache:'force-cache'}).then(function(x){return x.ok?x.arrayBuffer():null}).then(function(ab){if(!ab)return;
    var u=new Uint8Array(ab),dv=new DataView(ab),N=dv.getUint16(0,true),H=2+2*N+4*(N+1),out=new Uint8Array(N*1024);
    for(var i=0;i<N;i++){var a=H+dv.getUint32(2+2*N+4*i,true),b=H+dv.getUint32(2+2*N+4*(i+1),true),o=i*1024;
      while(a<b&&o<(i+1)*1024){var c=u[a++];if(c&128){var n=(c&127)+1,v=u[a++];while(n--)out[o++]=v}else{var m=c+1;while(m--)out[o++]=u[a++]}}}
    var idx={};for(var k=0;k<N;k++)idx[dv.getUint16(2+2*k,true)]=k;BMP=out;BMI=idx}).catch(function(){})})();
/* ---------- Robot OLED 128x64: mặt kiểu thạch (Mochi) ----------
   Cùng thuật toán với firmware ESP32 v7.2 (phần OLED): mắt khối tròn to, miệng cười dày nhỏ,
   thông số chạy bằng lò xo nên nảy như thạch; đổi kiểu hình thì thu lại rồi bật ra.
   Vẽ lên canvas 128x64 rồi lọc ngưỡng 2 màu như màn OLED thật. */
var ROBOT=(function(){
"use strict";
var FONT=[[0,0,0,0,0,0,0,0],[4,4,4,4,4,0,4,0],[10,10,10,0,0,0,0,0],[10,10,31,10,31,10,10,0],[4,15,20,14,5,30,4,0],[24,25,2,4,8,19,3,0],[8,20,20,8,21,18,13,0],[6,6,4,8,0,0,0,0],[2,4,8,8,8,4,2,0],[8,4,2,2,2,4,8,0],[4,21,14,31,14,21,4,0],[0,4,4,31,4,4,0,0],[0,0,0,0,6,6,4,8],[0,0,0,31,0,0,0,0],[0,0,0,0,0,6,6,0],[0,1,2,4,8,16,0,0],[14,17,19,21,25,17,14,0],[4,12,4,4,4,4,14,0],[14,17,1,14,16,16,31,0],[31,1,2,6,1,17,14,0],[2,6,10,18,31,2,2,0],[31,16,30,1,1,17,14,0],[7,8,16,30,17,17,14,0],[31,1,1,2,4,8,16,0],[14,17,17,14,17,17,14,0],[14,17,17,15,1,2,28,0],[0,0,4,0,4,0,0,0],[0,0,4,0,4,4,8,0],[1,2,4,8,4,2,1,0],[0,0,31,0,31,0,0,0],[8,4,2,1,2,4,8,0],[14,17,1,6,4,0,4,0],[14,17,21,23,22,16,15,0],[4,10,17,17,31,17,17,0],[30,17,17,30,17,17,30,0],[14,17,16,16,16,17,14,0],[30,17,17,17,17,17,30,0],[31,16,16,30,16,16,31,0],[31,16,16,30,16,16,16,0],[15,17,16,16,19,17,15,0],[17,17,17,31,17,17,17,0],[14,4,4,4,4,4,14,0],[7,2,2,2,2,18,12,0],[17,18,20,24,20,18,17,0],[16,16,16,16,16,16,31,0],[17,27,21,21,21,17,17,0],[17,17,25,21,19,17,17,0],[14,17,17,17,17,17,14,0],[30,17,17,30,16,16,16,0],[14,17,17,17,21,18,13,0],[30,17,17,30,20,18,17,0],[14,17,16,14,1,17,14,0],[31,21,4,4,4,4,4,0],[17,17,17,17,17,17,14,0],[17,17,17,17,17,10,4,0],[17,17,17,21,21,21,10,0],[17,17,10,4,10,17,17,0],[17,17,10,4,4,4,4,0],[31,1,2,14,8,16,31,0],[15,8,8,8,8,8,15,0],[0,16,8,4,2,1,0,0],[15,1,1,1,1,1,15,0],[4,10,17,0,0,0,0,0],[0,0,0,0,0,0,31,0],[12,12,4,2,0,0,0,0],[0,0,12,2,14,18,15,0],[16,16,22,25,17,25,22,0],[0,0,14,17,16,17,14,0],[1,1,13,19,17,19,13,0],[0,0,14,17,31,16,14,0],[2,5,4,14,4,4,4,0],[0,0,14,19,19,13,1,14],[16,16,22,25,17,17,17,0],[4,0,12,4,4,4,14,0],[2,0,2,2,2,18,12,0],[16,16,18,20,24,20,18,0],[12,4,4,4,4,4,14,0],[0,0,26,21,21,21,21,0],[0,0,22,25,17,17,17,0],[0,0,14,17,17,17,14,0],[0,0,22,25,25,22,16,16],[0,0,13,19,19,13,1,1],[0,0,22,25,16,16,16,0],[0,0,15,16,14,1,30,0],[4,4,31,4,4,5,2,0],[0,0,17,17,17,19,13,0],[0,0,17,17,17,10,4,0],[0,0,17,17,21,21,10,0],[0,0,17,10,4,10,17,0],[0,0,17,17,15,1,17,14],[0,0,31,2,4,8,31,0],[2,4,4,8,4,4,2,0],[4,4,4,0,4,4,4,0],[8,4,4,2,4,4,8,0],[8,21,2,0,0,0,0,0]];
var cv=$('#oled'),cx=cv.getContext('2d',{willReadFrequently:true});
var big=$('#oledBig'),bx=big?big.getContext('2d'):null;
var FGC=[143,240,248],BGC=[3,7,10],g=cx;
function ptext(s,x,y){x=Math.round(x);y=Math.round(y);
  for(var i=0;i<s.length;i++){var c=s.charCodeAt(i)-32;if(c<0||c>94)c=31;var f=FONT[c];
    for(var r=0;r<8;r++)for(var k=0;k<5;k++)if(f[r]&(16>>k))g.fillRect(x+i*6+k,y+r,1,1)}}
function ctext(s,y){s=String(s).slice(0,21);ptext(s,(128-s.length*6+1)/2,y)}
function ctxt(s,y){ctext(s,y)}
function line(a,b,c,d,w){g.lineWidth=w||3;g.beginPath();g.moveTo(a,b);g.lineTo(c,d);g.stroke()}
function quad(x0,y0,qx,qy,x1,y1,w){g.lineWidth=w||3;g.beginPath();g.moveTo(x0,y0);g.quadraticCurveTo(qx,qy,x1,y1);g.stroke()}
function circ(x,y,r,fill,w){if(r<=0)return;g.beginPath();g.arc(x,y,r,0,Math.PI*2);if(fill)g.fill();else{g.lineWidth=w||3;g.stroke()}}

/* ---- mắt và miệng ---- */
function blobPath(x,y,w,h){var a=w/2,b=h/2,n=3.4;g.beginPath();
  for(var i=0;i<=48;i++){var th=i/48*Math.PI*2,c=Math.cos(th),s=Math.sin(th);
    var px=a*Math.sign(c)*Math.pow(Math.abs(c),2/n),py=b*Math.sign(s)*Math.pow(Math.abs(s),2/n);
    px*=1-0.07*Math.max(0,py/b);if(i)g.lineTo(x+px,y+py);else g.moveTo(x+px,y+py)}g.closePath()}
function drawEye(e,side){var x=e.x,y=e.y,w=Math.max(2,e.w),h=Math.max(2,e.h);
  if(e.k==='chev'){var s=e.s,d=side;line(x-d*w/2,y-s,x+d*w/2,y,4.5);line(x+d*w/2,y,x-d*w/2,y+s,4.5);return}   /* > < khi cười to */
  if(e.k==='arch'){var r=w/2,tp=y-h/2+r;g.lineWidth=4.5;g.beginPath();g.moveTo(x-r,y+h/2);g.lineTo(x-r,tp);g.arc(x,tp,r,Math.PI,0);g.lineTo(x+r,y+h/2);g.stroke();return}   /* mắt cười nhắm ∩ */
  if(e.k==='heart'){heartShape(x,y,w);return}
  if(e.k==='ring'){g.lineWidth=3;g.beginPath();g.ellipse(x,y,w/2,h/2,0,0,Math.PI*2);g.stroke();return}           /* mắt tròn rỗng */
  if(e.k==='ang'){var r=side*.38,ux=Math.cos(r),uy=Math.sin(r);                                                  /* giận: lông mày xéo xuống phía trong + nửa mắt rỗng */
    line(x-side*ux*w*.55,y-side*uy*w*.55,x+side*ux*w*.85,y+side*uy*w*.85,3);
    g.lineWidth=3;g.beginPath();g.ellipse(x,y,w/2,h,r,0,Math.PI);g.stroke();return}
  blobPath(x,y,w,h);g.fill();
  g.fillStyle='#000';var top=y-h/2,inX=x+side*w/2,outX=x-side*w/2;
  if(e.ti>0.01){g.beginPath();g.moveTo(inX+side*2,top-2);g.lineTo(inX+side*2,top+e.ti*h);g.lineTo(outX-side*2,top-2);g.fill()}
  if(e.to>0.01){g.beginPath();g.moveTo(outX-side*2,top-2);g.lineTo(outX-side*2,top+e.to*h);g.lineTo(inX+side*2,top-2);g.fill()}
  if(e.bc>0.01){g.beginPath();g.ellipse(x,y+h*.95-h*.55*e.bc,w*.62,h*.62,0,0,Math.PI*2);g.fill()}
  if(e.tc>0.01){g.beginPath();g.ellipse(x,y-h*.95+h*.55*e.tc,w*.62,h*.62,0,0,Math.PI*2);g.fill()}
  g.fillStyle='#fff'}
function drawMouth(m){var x=m.x,y=m.y;
  if(m.k==='arc')quad(x-m.w,y-m.c*.5,x,y+m.c*1.5,x+m.w,y-m.c*.5,3.6);
  else if(m.k==='laugh'){var top=y-m.h*.4;g.lineWidth=3.5;g.beginPath();g.moveTo(x-m.w,top);g.lineTo(x+m.w,top);g.ellipse(x,top,m.w,m.h,0,0,Math.PI);g.closePath();g.stroke();
    g.beginPath();g.ellipse(x,top+m.h-1.5,m.w*.5,m.h*.32,0,Math.PI,Math.PI*2);g.fill()}                                      /* miệng cười há to có lưỡi */
  else if(m.k==='caret'){line(x-m.w,y+m.h/2,x,y-m.h/2,3);line(x,y-m.h/2,x+m.w,y+m.h/2,3)}                                   /* miệng ^ khi giận */
  else if(m.k==='D'){var t2=y-m.h*.4;g.beginPath();g.moveTo(x-m.w,t2);g.lineTo(x+m.w,t2);g.ellipse(x,t2,m.w,m.h,0,0,Math.PI);g.fill()}
  else if(m.k==='o'){g.lineWidth=3;g.beginPath();g.ellipse(x,y,m.rx,m.ry,0,0,Math.PI*2);g.stroke()}
  else if(m.k==='zig'){g.lineWidth=3;g.beginPath();for(var i=0;i<=4;i++){var px=x-m.w+i*m.w/2,py=y+(i%2?m.a:-m.a);if(i)g.lineTo(px,py);else g.moveTo(px,py)}g.stroke()}}
/* dấu 💢 (gân nổi) */
function vein(x,y,s){g.lineWidth=2;[[-1,-1],[1,-1],[-1,1],[1,1]].forEach(function(q){var a=q[0],b=q[1];g.beginPath();g.moveTo(x+a*s*1.7,y+b*s*.45);g.lineTo(x+a*s*.45,y+b*s*.45);g.lineTo(x+a*s*.45,y+b*s*1.7);g.stroke()})}
/* mắt trái tim (chủ nhà) */
function heartShape(x,y,s){var r=s*.28;g.beginPath();g.arc(x-s*.24,y-s*.12,r,0,Math.PI*2);g.arc(x+s*.24,y-s*.12,r,0,Math.PI*2);g.fill();
  g.beginPath();g.moveTo(x-s*.5,y-s*.04);g.lineTo(x+s*.5,y-s*.04);g.lineTo(x,y+s*.5);g.closePath();g.fill()}
/* thẻ thu nhỏ được (tâm cx, cy) */
function cardS(cx,cy,sc){if(sc<.08)return;var w=20*sc,h=14*sc;g.strokeStyle='#fff';g.lineWidth=1;g.strokeRect(Math.round(cx-w/2)+.5,Math.round(cy-h/2)+.5,Math.round(w),Math.round(h));
  if(sc>.5)g.fillRect(Math.round(cx-w/2+3*sc),Math.round(cy-h/2+4*sc),Math.round(5*sc),Math.round(4*sc))}
/* CHỦ NHÀ: mắt dẹt quét thẻ → giật mình nhận ra → mắt trái tim đập thình thịch, vương miện rơi xuống → nháy mắt */
function ownerFace(F,e){
  if(e<800){F.E=pair('blob',{w:28,h:6,y:32});F.gx=Math.sin(e/130)*6;F.m=M('arc',{w:6,c:0,y:47});X.push(function(){g.fillRect(Math.round(12+e/800*104),14,1,30)});return 0}
  if(e<1000){F.E=pair('blob',{w:25,h:38,y:29});F.m=M('o',{rx:4,ry:5,y:47});X.push(function(){spark(14,10,e/60);spark(114,10,e/60+2)});return 1}
  var cy=Math.round(Math.min(1,-14+(e-1000)/16));
  if(e<2700){var b=(e-1000)%900,beat=(b<130||(b>240&&b<370))?1:0;F.E=pair('heart',{w:23+beat*5,y:28});F.m=M('D',{w:7,h:8,y:47});
    X.push(function(){crown(64,cy,18,10);var p=((e-1000)%1400)/1400;heartShape(12,40-p*30,4+p*2);heartShape(116,46-p*30,4+p*2)});return 2}
  F.E=pair('arch',{w:24,h:22,y:27});F.m=M('D',{w:10,h:11,y:43});X.push(function(){crown(64,1,18,10)});return 3}
/* THÊM THẺ: thẻ rơi xuống, robot há miệng "ăn" thẻ, nhai nhóp nhép, rồi cười híp mắt kèm +1 lấp lánh */
function eatFace(F,e){
  if(e<500){var p=e/500;F.E=pair('blob',{h:35,y:28});F.gy=-1;F.m=M('o',{rx:3+p*2,ry:4+p*3,y:46});X.push(function(){cardS(64,Math.round(-10+p*26),1)});return 0}
  if(e<800){var q=(e-500)/300;F.E=pair('blob',{h:36,y:28});F.m=M('o',{rx:6,ry:7,y:46});X.push(function(){cardS(64,16+q*28,1-q)});return 1}
  if(e<1300){var c=Math.floor((e-800)/120)%2;F.E=pair('arch',{w:24,h:20,y:28});F.m=M('arc',{w:c?5:8,c:c?3:1,y:c?47:45});F.gy=c?1:-1;return 2}
  F.E=pair('arch',{w:24,h:22,y:27});F.m=M('D',{w:11,h:12,y:43});F.gy=e<1500?-3:0;
  X.push(function(){var k=Math.min(1,(e-1300)/200);spark(14,10,e/80);spark(114,12,e/80+1.5);spark(30,52,e/80+3);spark(98,52,e/80+.7);if(k>.3)ctxt('+1',Math.round(10-k*6))});return 3}
/* cười to theo mẫu: mắt bị má đẩy lên → híp thành vạch cong → bật thành > < và miệng há (cười ha ha) → híp lại → mắt thường */
function laughFace(F,e,endAt){
  if(e<160){F.E=pair('blob',{bc:.55,h:28,y:33});F.m=M('arc',{w:9,c:2})}
  else if(e<340){F.E=pair('blob',{w:30,h:10,tc:1,y:34});F.m=M('arc',{w:7,c:2.5})}
  else if(e<endAt){F.E=[P('chev',{x:33,y:24,w:28,s:11}),P('chev',{x:95,y:24,w:28,s:11})];F.m=M('laugh',{w:12,h:15+Math.sin(e/70)*2,y:40});F.gy=-Math.abs(Math.sin(e/170))*2}
  else if(e<endAt+250){F.E=pair('blob',{w:30,h:10,tc:1,y:34});F.m=M('arc',{w:8,c:2})}
  else{F.E=pair('blob');F.m=M('arc',{w:11,c:5.5})}}
/* giận theo mẫu: mắt nghiêng, mếu → mắt rỗng + lông mày xéo + dấu 💢 → phóng to rung lắc → nhắm thành vạch "-_-" */
function angryFace(F,e){
  if(e<250){F.E=pair('blob',{ti:.3,h:30,y:32});F.m=M('arc',{w:7,c:-3,y:47});return 0}
  if(e<1800){F.E=pair('ang',{w:22,h:9,y:30});F.m=M('caret',{w:4,h:4,y:45});F.gx=Math.sin(e/40);
    if(e>=1100){F.z=1.8;F.gx=Math.sin(e/25)*3;F.gy=Math.cos(e/31)*2;X.push(function(){vein(64,7,3.4+Math.sin(e/60))});return 2}
    X.push(function(){vein(64,8,3.4+Math.abs(Math.sin(e/90))*.8)});return 1}
  F.E=pair('blob',{w:24,h:3,y:33});F.m=M('arc',{w:6,c:0,y:47});return 3}
var LX=36,RX=92,EY=31;
function P(k,o){var e={k:k,x:0,y:EY,w:22,h:33,ti:0,to:0,bc:0,tc:0,s:13};for(var i in o)e[i]=o[i];return e}
function M(k,o){var m={k:k,x:64,y:45,w:10,c:4,h:9,rx:3.5,ry:4,a:2};for(var i in o)m[i]=o[i];return m}
function pair(k,o,o2){var L=P(k,o),R=P(k,o2||o);L.x+=LX;R.x+=RX;return [L,R]}

/* ---- phụ kiện ---- */
function spark(x,y,f){var r=[1,3,2,0][Math.floor(f)%4];if(r){g.fillRect(x-r,y,2*r+1,1);g.fillRect(x,y-r,1,2*r+1)}}
function card(x,y,cross){g.strokeStyle='#fff';g.lineWidth=1;g.strokeRect(x+.5,y+.5,19,13);g.fillRect(x+3,y+4,5,4);g.fillRect(x+10,y+4,7,1);g.fillRect(x+10,y+7,5,1);
  if(cross){line(x-1,y-1,x+21,y+15,2);line(x+21,y-1,x-1,y+15,2)}}
function crown(x,y,w,h){g.beginPath();g.moveTo(x-w/2,y+h);g.lineTo(x-w/2,y+2);g.lineTo(x-w/4,y+h*.55);g.lineTo(x,y);g.lineTo(x+w/4,y+h*.55);g.lineTo(x+w/2,y+2);g.lineTo(x+w/2,y+h);g.closePath();g.fill()}
function phone(x,y){g.strokeStyle='#fff';g.lineWidth=1;g.strokeRect(x+.5,y+.5,11,18);g.fillRect(x+4,y+15,4,1);g.fillRect(x+3,y+3,6,10)}
function lock(x,y,open){g.lineWidth=2;g.beginPath();g.arc(x+(open?4:0),y+7,5,Math.PI,0);g.stroke();g.fillRect(x-7,y+7,14,11)}
function tear(x,y){g.beginPath();g.moveTo(x,y-4);g.quadraticCurveTo(x+3,y,x,y+3);g.quadraticCurveTo(x-3,y,x,y-4);g.fill()}

/* ---- bảng xem thử ---- */
var GALLERY=[
  ['idle','Bình thường','Chớp mắt kiểu thạch, liếc, lười, cười híp'],['curious','Tò mò','Có người cách 40–80 cm: mắt to, cười, lấp lánh'],
  ['attentive','Có người đến','Mắt to → nhìn xuống đầu đọc → cười mời quẹt'],['happy','Thành viên','Híp mắt → > < cười ha ha → chào tên'],
  ['remote','Mở từ xa','Nhắm mắt cười ∩ ∩, hình điện thoại'],['owner','Chủ nhà ★','Chớp mắt → mắt trái tim đập → cười ∩ ∩'],
  ['guard','Cấp quyền','Mắt nghiêm, thẻ nhấp nháy, thanh thời gian'],['excited','Thêm thẻ','Há miệng ăn thẻ, nhai, cười +1'],
  ['sad','Thu hồi thẻ','Mắt cụp, miệng mếu, rơi nước mắt'],['angry','Thẻ lạ','Mắt rỗng mày xéo, gân nổi, phóng to rung → -_-'],
  ['locked','Khóa ★','Sai 3 lần: ngủ luôn, bong bóng mũi, ổ khóa ở góc'],['shocked','Báo động','Rung mạnh: mắt tròn rỗng, miệng tròn'],
  ['wake','Thức dậy ★','Hết khóa: bong bóng vỡ, mở mắt chớp'],
  ['cute','Vui vẻ','Robot hướng dẫn trong nhà: nhún nhảy, cười híp']];
var GAL_TXT={happy:'Xin chao Lan',excited:'+ Thanh vien 2',sad:'DA THU HOI',angry:'Sai the 1/3'};
var R={face:'idle',since:0,blinkAt:-1e9,blinkNext:0,idleSub:0,idleUntil:0,idleNext:4000,look:1,wakeUntil:0,prev:'idle'};
S.lastAct=Date.now();S.preview=null;

function pickFace(now){
  var m=S.mode;
  if(m==='ALARM'||m==='FIRE')return 'shocked';
  if(m==='READY')return 'attentive';
  if(m==='GRANTED')return S.greet==='Mo tu xa'?'remote':'happy';
  if(m==='OWNER')return 'owner';
  if(m==='GRANT')return 'guard';
  if(m==='GRANT_OK')return 'excited';
  if(m==='GRANT_DEL')return 'sad';
  if(m==='DENIED')return 'angry';
  if(m==='LOCKED')return 'locked';
  if(S.cute)return 'cute';
  if(S.dist>=0&&S.dist<=80){S.lastAct=now;return 'curious'}
  return 'idle';
}
function faceText(f,e,now){
  if(S.preview&&now<S.preview.until&&GAL_TXT[f])return GAL_TXT[f];
  var d=new Date();
  switch(f){
    case 'idle':return pad(d.getHours())+':'+pad(d.getMinutes());
    case 'attentive':return e>=1900?'Moi quet the':'';
    case 'happy':return e>=340?(S.greet||'Xin chao').slice(0,21):'';
    case 'remote':return e>500?'Mo tu xa':'';
    case 'owner':return e>1300&&e<2700?'CHU NHA':e>=2700?'Chao chu nha!':'';
    case 'excited':return (/^\+/.test(S.greet)?S.greet:'+ '+ascii(S.greet||'')).slice(0,21);
    case 'sad':return 'DA THU HOI';
    case 'angry':if(e>=1100&&e<1800)return '';return S.fails?'Sai the '+S.fails+'/3':(S.sysLock?'He thong dang khoa':'Tu choi');
    case 'locked':return 'Khoa the '+(S.preview&&now<S.preview.until?30:secLeft(S.until))+'s';
    case 'shocked':return S.mode==='FIRE'?'THOAT HIEM':'BAO DONG';
  }
  return '';
}
var X=[];
function target(f,e,now){
  var F={E:pair('blob'),m:M('arc'),gx:0,gy:Math.sin(now/900)*1.2},L=F.E[0],Rr=F.E[1],m=F.m;X=[];
  function both(o){for(var k in o){L[k]=o[k];Rr[k]=o[k]}}
  if(f==='idle'||f==='cute'){
    if(now>R.idleNext){var r=Math.random();R.idleSub=f==='cute'?(r<.5?4:3):(r<.35?1:r<.6?3:r<.9?4:0);R.look=Math.random()<.5?-1:1;R.idleUntil=now+1500+Math.random()*1300;R.idleNext=R.idleUntil+(f==='cute'?900:2200)+Math.random()*2000}
    var s=now<R.idleUntil?R.idleSub:0;
    if(s===1){F.gx=R.look*9;(R.look<0?L:Rr).h=35;(R.look<0?Rr:L).h=31;m.x+=R.look*3}
    else if(s===2){both({w:27,h:11,y:34});m.w=6;m.c=-1.6;m.y=47}
    else if(s===3){var q=Math.floor(now/260)%2;both(q?{w:25,h:29,y:33}:{w:20,h:36,y:30})}
    else if(s===4){F.E=pair('arch',{w:24,h:22,y:27});F.m=M('D',{w:10,h:11,y:43});m=F.m;if(f==='cute')F.gy=-Math.abs(Math.sin(now/220))*3}
    if(f==='cute'){m.w=Math.max(m.w,11);m.c=Math.max(m.c,5)}
  }
  else if(f==='curious'){L.w=24;L.h=36;L.y=29;Rr.w=20;Rr.h=25;Rr.y=34;F.m=M('arc',{x:66,w:10,c:5});F.gx=-3;F.gy=-Math.abs(Math.sin(now/300))*1.5;X.push(function(){spark(16,10,now/110);spark(112,12,now/110+2)})}   /* có người: tò mò mà vui */
  else if(f==='attentive'){
    if(e<700){both({w:25,h:38,y:29});F.m=M('D',{w:8,h:8,y:44});X.push(function(){spark(18,10,e/90);spark(110,10,e/90+2)})}
    else if(e<1900){both({w:24,h:28,y:36});m.w=6;m.c=0;m.y=49}
    else{m.w=11;m.c=5;X.push(function(){if(Math.floor(now/500)%2)card(54,3,0)})}}
  else if(f==='happy')laughFace(F,e,2000);
  else if(f==='remote'){F.E=pair('arch',{w:24,h:22,y:27});F.m=M('D',{w:10,h:11,y:43});X.push(function(){phone(58,4+Math.round(Math.sin(now/200)))})}
  else if(f==='owner')ownerFace(F,e);
  else if(f==='guard'){both({ti:.2,to:.2,h:30,y:32});m.w=7;m.c=0;m.y=48;
    var left=S.preview&&now<S.preview.until?15000-(e%15000):Math.max(0,S.grantUntil-now);
    X.push(function(){var fast=left<5000,on=fast?Math.floor(now/150)%2:Math.floor(now/600)%3!==0;if(on)card(54,17,0);ctext('THEM THE '+Math.ceil(left/1000)+'s',2);
      g.strokeStyle='#fff';g.lineWidth=1;g.strokeRect(14.5,58.5,99,4);g.fillRect(16,60,Math.round(96*left/15000),2)})}
  else if(f==='excited')eatFace(F,e);
  else if(f==='sad'){both({to:.42,h:29,y:34});m.w=8;m.c=-3.5;m.y=50;F.gy=1.5;
    X.push(function(){card(54,Math.round(-14+Math.min(16,e/50)),1);if(e>600)tear(LX-6,50+((e/16)%12))})}
  else if(f==='angry')angryFace(F,e);
  else if(f==='locked'){both({w:26,h:4,y:33});m.w=5;m.c=0;m.y=48;X.push(function(){lock(116,1,0)})}
  else if(f==='shocked'){F.E=pair('ring',{w:20,h:28,y:28});F.m=M('o',{rx:6,ry:7,y:46});F.gx=Math.sin(now/30)*2.5;X.push(function(){if(Math.floor(now/180)%2)ctext('!',4)})}
  else if(f==='wake'){both({w:25,h:38,y:29});F.m=M('o',{rx:4,ry:5,y:47})}
  else if(f==='sleepy'){both({w:27,h:11,y:35});m.w=5;m.c=-1.2;m.y=48;F.gy=Math.sin(now/1400)*1.5}
  else if(f==='asleep'){both({tc:1,w:24,h:22,y:38});m.w=4;m.c=0;m.y=50;F.gy=Math.sin(now/1400)*2;
    X.push(function(){var p=(now%2400)/2400,q=((now+1200)%2400)/2400;ptext('z',100+p*12,22-p*16);ptext('Z',96+q*16,26-q*20)})}
  var canBlink=F.E[0].k==='blob'&&F.E[0].h>20&&F.E[0].tc<.1&&(F.E[0].bc<.1||F.E[1].bc<.1);
  if(canBlink&&now>R.blinkNext){R.blinkAt=now;R.blinkNext=now+(Math.random()<.2?420:2400+Math.random()*2600)}
  if(f==='guard'&&S.grantUntil-now<5000&&R.blinkNext>now+700)R.blinkNext=now+500+Math.random()*300;
  if(canBlink&&now-R.blinkAt<120)F.E.forEach(function(p){if(p.bc<.1){p.w*=1.3;p.h=4;p.y+=3}});
  return F;
}

/* ---- lò xo + đổi kiểu hình ---- */
var SK=320,SD=2*Math.sqrt(320)*.42;
function clone(o){return JSON.parse(JSON.stringify(o))}
function back(q){var s=2.2;q-=1;return q*q*((s+1)*q+s)+1}
function spring(c,tg,dt){var s=dt/1000;for(var k in tg)if(typeof tg[k]==='number'){var v=c.v[k]||0,x=c.p[k];v+=((tg[k]-x)*SK-v*SD)*s;c.p[k]=x+v*s;c.v[k]=v}}
function blendPart(c,tg,dt){
  if(!c)return {p:clone(tg),v:{},sq:1,ph:0,next:null};
  if(c.next||c.p.k!==tg.k){
    if(!c.next){c.next=clone(tg);c.ph=0}else if(c.next.k!==tg.k)c.next=clone(tg);else for(var k in tg)if(typeof tg[k]==='number')c.next[k]=tg[k];
    c.ph+=dt;if(c.ph<100)c.sq=1-c.ph/100;else{c.p=clone(c.next);c.v={};var q=Math.min(1,(c.ph-100)/200);c.sq=back(q);if(q>=1){c.next=null;c.sq=1}}
    return c}
  spring(c,tg,Math.min(dt,40));return c}
function part(c,fn){if(c.sq<=0.02)return;var p=c.p;g.save();g.translate(p.x,p.y);g.scale(c.sq,c.sq);g.translate(-p.x,-p.y);fn();g.restore()}
/* ---- BIỂU CẢM ẢNH (bộ hình @khoi2mai): buồn ngủ, ngủ, thức dậy, mắt trái tim ---- */
function bmpFrame(f,e){var k=Math.floor(e/42);                 /* 24 khung/giây */
  if(f==='locked')return k<25?151+k:176+(k-25)%69;               /* khóa: mắt khép → ngủ, bong bóng mũi phồng xẹp (lặp) */
  if(f==='wake')return Math.min(244+k,271);                       /* bong bóng vỡ → mở mắt, chớp */
  if(f==='owner'&&e<2700){if(k<24)return 490+k;var p=(k-24)%18;return p<9?503+p:521-p}   /* chớp mắt → mắt trái tim đập */
  return -1}
function drawBmp(n,f,e,now){var o=BMI[n]*1024,img=cx.createImageData(128,64),d=img.data;
  for(var y=0;y<64;y++)for(var x=0;x<128;x++){var on=BMP[o+y*16+(x>>3)]>>(7-(x&7))&1,c=on?FGC:BGC,i=(y*128+x)*4;d[i]=c[0];d[i+1]=c[1];d[i+2]=c[2];d[i+3]=255}
  cx.putImageData(img,0,0);
  var txt=faceText(f,e,now);if(txt){var s=String(txt).slice(0,21),w=s.length*6+3;cx.fillStyle='rgb('+BGC+')';cx.fillRect(Math.floor((128-w)/2),54,w,10);cx.fillStyle='rgb('+FGC+')';ctext(s,56)}
  if(bx)bx.putImageData(cx.getImageData(0,0,128,64),0,0);
  var fe=$('#face');if(fe)fe.textContent=f+' · khung '+n;
  if(f==='locked'){cx.fillStyle=cx.strokeStyle='rgb('+FGC+')';lock(116,1,0)}}   /* ổ khóa ở góc, không che mặt */
var cur={L:null,R:null,m:null},gz={p:{gx:0,gy:0,z:1},v:{}},lastF=0;
function drawRobot(t){
  var now=Date.now(),dt=Math.min(100,lastF?t-lastF:33);lastF=t;
  var f;
  if(S.preview&&now<S.preview.until)f=S.preview.expr;
  else{
    if(S.preview){S.preview=null;paintGallery()}
    f=pickFace(now);
    if(R.prev==='locked'&&f!=='locked'){R.wakeUntil=now+WAKE_MS;R.prev=f}   /* sửa lỗi cũ: nhớ đã thức, không kéo dài mặt thức dậy mãi */   /* bị đánh thức */
    if(now<R.wakeUntil&&(f==='idle'||f==='curious'||f==='attentive'||f==='cute'))f='wake';   /* quẹt thẻ, báo động: phản ứng ngay, không chờ thức dậy */
  }
  if(f!=='wake')R.prev=f;
  var pk=f+(S.preview?S.preview.until:'');
  if(pk!==R.key){if(!(R.face==='wake'&&(f==='attentive'||f==='curious')))R.since=now;R.key=pk;R.face=f}
  var e=now-R.since,bf=bmpFrame(f,e);
  if(bf>=0&&BMI[bf]!==undefined){drawBmp(bf,f,e,now);return}   /* chưa tải được ảnh thì vẽ mặt thạch */
  var F=target(f,e,now);
  cur.L=blendPart(cur.L,F.E[0],dt);cur.R=blendPart(cur.R,F.E[1],dt);cur.m=blendPart(cur.m,F.m,dt);spring(gz,{gx:F.gx,gy:F.gy,z:F.z||1},Math.min(dt,40));
  g.fillStyle='#000';g.fillRect(0,0,128,64);g.fillStyle='#fff';g.strokeStyle='#fff';g.lineCap='round';g.lineJoin='round';
  g.save();g.translate(Math.round(gz.p.gx),Math.round(gz.p.gy));g.translate(64,30);g.scale(gz.p.z,gz.p.z);g.translate(-64,-30);
  part(cur.L,function(){drawEye(cur.L.p,1)});part(cur.R,function(){drawEye(cur.R.p,-1)});part(cur.m,function(){drawMouth(cur.m.p)});
  g.restore();g.fillStyle='#fff';g.strokeStyle='#fff';X.forEach(function(fn){fn()});
  var txt=faceText(f,e,now);if(txt)ctext(txt,56);
  var invert=f==='shocked'&&Math.floor(now/260)%2===1;
  var img=cx.getImageData(0,0,128,64),d=img.data,on=invert?BGC:FGC,off=invert?FGC:BGC;
  for(var i=0;i<d.length;i+=4){var c=d[i]>110?on:off;d[i]=c[0];d[i+1]=c[1];d[i+2]=c[2];d[i+3]=255}
  cx.putImageData(img,0,0);if(bx)bx.putImageData(img,0,0);
  var fe=$('#face');if(fe)fe.textContent=f;
}

/* ---------- Bảng xem thử biểu cảm ---------- */
function paintGallery(){
  var box=$('#gallery');if(!box)return;
  var on=S.preview&&Date.now()<S.preview.until?S.preview.expr:null;
  box.innerHTML=GALLERY.map(function(x){
    return '<button class="gx" type="button" data-act="face" data-x="'+x[0]+'" aria-pressed="'+(on===x[0])+'"><b>'+x[1]+'</b><span>'+x[2]+'</span></button>';
  }).join('');
  var gn=$('#galNow');if(gn)gn.textContent=on?'Đang xem: '+GALLERY.filter(function(x){return x[0]===on})[0][1]+' · bấm lần nữa để quay lại':'Đang chạy theo trạng thái cửa';
}

return {draw:drawRobot,gallery:paintGallery,font:FONT};
})();
var FONT=ROBOT.font;
function drawRobot(t){ROBOT.draw(t)}
function paintGallery(){ROBOT.gallery()}

</script>
<script>
(function(){
'use strict';
/* ================= NGUỒN DỮ LIỆU =================
   Chỉ dùng dữ liệu thật từ ESP32: /api/poll, /api/act, /api/card, /api/home. */
function api(path){
  return fetch(path,{cache:'no-store'}).then(function(r){return r.json()});
}

/* ================= NGÔI NHÀ 2.5D: dựng hình lấy từ bản giả lập, dữ liệu lấy từ ESP32 ================= */
var H=null,VIEW='out',infoSel=null,labelsOn=true,sceneBuilt=false,sceneLastFr=0,lastGuide='';
var MODE_VN={0:'Ở nhà',1:'Vắng nhà',2:'Đi ngủ',3:'Chào mừng về nhà'};
function homeModeName(h){if(!h)return '—';if(h.fire)return 'CHÁY!';if(h.wake)return 'Báo thức';if(h.mode===1)return h.armed?'Vắng nhà · đang canh gác':'Vắng nhà · đếm ngược';return MODE_VN[h.mode]}
/* ================= DỰNG HÌNH 2.5D (isometric) ================= */
var IS=26,C30=0.8660254,S30=0.5;
function P(x,y,z){return [(x-y)*C30*IS,(x+y)*S30*IS-(z||0)*IS]}
function ps(a){return a.map(function(p){var q=P(p[0],p[1],p[2]);return q[0].toFixed(1)+','+q[1].toFixed(1)}).join(' ')}
function pg(a,f,x){return '<polygon points="'+ps(a)+'" fill="'+f+'"'+(x||'')+'/>'}
function box(x,y,z,w,d,h,t,l,r,x2){
  return pg([[x,y+d,z],[x+w,y+d,z],[x+w,y+d,z+h],[x,y+d,z+h]],l,x2)+pg([[x+w,y,z],[x+w,y+d,z],[x+w,y+d,z+h],[x+w,y,z+h]],r,x2)+pg([[x,y,z+h],[x+w,y,z+h],[x+w,y+d,z+h],[x,y+d,z+h]],t,x2)}
function qY(Y,x0,x1,z0,z1){return [[x0,Y,z0],[x1,Y,z0],[x1,Y,z1],[x0,Y,z1]]}
function qX(X,y0,y1,z0,z1){return [[X,y0,z0],[X,y1,z0],[X,y1,z1],[X,y0,z1]]}
function qZ(Z,x0,x1,y0,y1){return [[x0,y0,Z],[x1,y0,Z],[x1,y1,Z],[x0,y1,Z]]}
function icirc(p,r,f,x){var q=P(p[0],p[1],p[2]);return '<circle cx="'+q[0].toFixed(1)+'" cy="'+q[1].toFixed(1)+'" r="'+r+'" fill="'+f+'"'+(x||'')+'/>'}
function mY(p){var q=P(p[0],p[1],p[2]);return 'matrix('+C30+' '+S30+' 0 1 '+q[0].toFixed(1)+' '+q[1].toFixed(1)+')'}   /* chữ nằm trên mặt y = hằng */
function mX(p){var q=P(p[0],p[1],p[2]);return 'matrix('+C30+' '+(-S30)+' 0 1 '+q[0].toFixed(1)+' '+q[1].toFixed(1)+')'}  /* chữ nằm trên mặt x = hằng */
function tag(p,text,cls,dx,dy){var q=P(p[0],p[1],p[2]),w=text.length*6.4+14,x=q[0]+(dx||0),y=q[1]+(dy||0);
  return '<g class="tag '+(cls||'')+'"><line x1="'+q[0].toFixed(1)+'" y1="'+q[1].toFixed(1)+'" x2="'+x.toFixed(1)+'" y2="'+(y+9).toFixed(1)+'" stroke="#fff" stroke-width="1.2" opacity=".85"/><circle cx="'+q[0].toFixed(1)+'" cy="'+q[1].toFixed(1)+'" r="2.4" fill="#fff"/>'+
    '<rect x="'+(x-w/2).toFixed(1)+'" y="'+y.toFixed(1)+'" width="'+w.toFixed(1)+'" height="18" rx="9" fill="rgba(15,23,32,.82)"/><text x="'+x.toFixed(1)+'" y="'+(y+12.5).toFixed(1)+'" text-anchor="middle" font-size="11" font-weight="600" fill="#fff" font-family="system-ui,sans-serif">'+esc(text)+'</text></g>'}
function hs(id,poly,label){return '<g class="hs" data-c="'+id+'" tabindex="0" role="button" aria-label="'+esc(label)+'"><polygon points="'+ps(poly)+'" fill="transparent"/><polygon class="hl" points="'+ps(poly)+'"/></g>'}

/* ---------- NGOÀI SÂN: vùng an toàn giữa thời tận thế zombie ---------- */
var HWID=10,DP=6.5,WH=4;
var FX0=-3.3,FX1=14.3,FY0=-2.3,FY1=11.8;            /* hàng rào */
function fenceLine(a,b,gap){                        /* cọc nhọn + 2 thanh ngang + dây thép gai */
  var s='',dx=b[0]-a[0],dy=b[1]-a[1],len=Math.hypot(dx,dy),n=Math.round(len/1.1),i,t,x,y;
  function inGap(x,y){return gap&&x>gap[0]&&x<gap[1]}
  for(i=0;i<=n;i++){t=i/n;x=a[0]+dx*t;y=a[1]+dy*t;if(inGap(x,y))continue;
    s+=box(x-0.07,y-0.07,0,0.14,0.14,1.25,'#5d4430','#6f5038','#5a4230');
    var top=P(x,y,1.25),tp=P(x,y,1.55);s+='<polygon points="'+(top[0]-2.6).toFixed(1)+','+top[1].toFixed(1)+' '+(top[0]+2.6).toFixed(1)+','+top[1].toFixed(1)+' '+tp[0].toFixed(1)+','+tp[1].toFixed(1)+'" fill="#6f5038"/>'}
  [0.45,0.95].forEach(function(z){
    if(gap){s+='<polyline points="'+ps([[a[0],a[1],z],[gap[0],a[1],z]])+'" stroke="#7a573b" stroke-width="2.6" fill="none"/><polyline points="'+ps([[gap[1],a[1],z],[b[0],b[1],z]])+'" stroke="#7a573b" stroke-width="2.6" fill="none"/>'}
    else s+='<polyline points="'+ps([[a[0],a[1],z],[b[0],b[1],z]])+'" stroke="#7a573b" stroke-width="2.6" fill="none"/>'});
  var pts=[],m=Math.round(len/0.35);for(i=0;i<=m;i++){t=i/m;x=a[0]+dx*t;y=a[1]+dy*t;if(inGap(x,y))continue;pts.push([x,y,1.32+(i%2?0.1:-0.06)])}
  if(gap){var L1=pts.filter(function(p){return p[0]<=gap[0]}),L2=pts.filter(function(p){return p[0]>=gap[1]});
    [L1,L2].forEach(function(L){if(L.length>1)s+='<polyline points="'+ps(L)+'" stroke="#9aa0a6" stroke-width="1.1" fill="none"/>'})}
  else s+='<polyline points="'+ps(pts)+'" stroke="#9aa0a6" stroke-width="1.1" fill="none"/>';
  return s;
}
function deadTree(x,y,h){
  var s=box(x-0.15,y-0.15,0,0.3,0.3,h,'#4a3a2c','#3e3024','#33281e'),b=P(x,y,h),c='#3e3024';
  [[-26,-30],[22,-36],[-10,-48],[30,-14],[-34,-8]].forEach(function(d,i){var m=[b[0]+d[0]*0.5,b[1]+d[1]*0.5];
    s+='<polyline points="'+(b[0]).toFixed(1)+','+(b[1]+8).toFixed(1)+' '+m[0].toFixed(1)+','+m[1].toFixed(1)+' '+(b[0]+d[0]).toFixed(1)+','+(b[1]+d[1]).toFixed(1)+'" stroke="'+c+'" stroke-width="'+(3.2-i*0.3)+'" fill="none" stroke-linecap="round"/>'});
  return s;
}
function sandbags(x0,x1,y){var s='';for(var r=0;r<2;r++)for(var x=x0+(r?0.25:0);x<x1;x+=0.55){var q=P(x,y,0.22+r*0.3);s+='<ellipse cx="'+q[0].toFixed(1)+'" cy="'+q[1].toFixed(1)+'" rx="8.5" ry="5" fill="'+(r?'#b8a27a':'#a8916a')+'" stroke="#7d6a4b" stroke-width=".8"/>'}return s}
function zombie(id){
  return '<g class="zb" id="'+id+'"><g class="zbob">'+
    '<path class="zleg1" d="M-2.5 0 L-3 -10" stroke="#3d3a4a" stroke-width="3.2" stroke-linecap="round"/><path class="zleg2" d="M2.5 0 L3 -10" stroke="#34313f" stroke-width="3.2" stroke-linecap="round"/>'+
    '<path d="M-6 -9 L6 -9 L5.2 -22 L-5.2 -22 Z" fill="#5f6b4e"/><path d="M-6 -9 L-3 -12 L0 -9 L3 -12 L6 -9 Z" fill="#4a543c"/>'+
    '<path class="zarm" d="M-3 -20 L11 -18 M3 -21 L13 -21.5" stroke="#8fbf7a" stroke-width="2.6" stroke-linecap="round"/>'+
    '<circle cx="1.5" cy="-27" r="5.6" fill="#8fbf7a"/><path d="M-3.6 -30 L-1 -32.4 L2 -31 L5 -32.6 L6.6 -29.5" stroke="#3a3a2a" stroke-width="1.6" fill="none"/>'+
    '<circle cx="4" cy="-27.6" r="1.1" fill="#3a0d0d"/><circle cx="0.6" cy="-27.4" r="1" fill="#3a0d0d"/><path d="M0.6 -24 L4.8 -24.4" stroke="#3a0d0d" stroke-width="1"/></g></g>';
}
var ZOMBIES=[
  {id:'z0',a:[-2.6,12.2],b:[4.6,12.2],sp:0.00045,ph:0},
  {id:'z1',a:[8.6,12.2],b:[14.4,12.2],sp:0.0005,ph:0.6},
  {id:'z2',a:[14.75,1.5],b:[14.75,10.5],sp:0.0004,ph:0.3},
  {id:'z3',a:[-3.75,0],b:[-3.75,9.5],sp:0.00042,ph:0.8},
  {id:'z4',a:[1.5,-2.75],b:[11.5,-2.75],sp:0.00038,ph:0.45}
];
function buildOut(){
  var s='<defs><radialGradient id="lampG"><stop offset="0" stop-color="#ffe7a3" stop-opacity=".9"/><stop offset="1" stop-color="#ffe7a3" stop-opacity="0"/></radialGradient>'+
    '<radialGradient id="fireG"><stop offset="0" stop-color="#ff6a2b" stop-opacity=".8"/><stop offset="1" stop-color="#ff6a2b" stop-opacity="0"/></radialGradient>'+
    '<radialGradient id="moonG"><stop offset="0" stop-color="#ff5a3c" stop-opacity=".55"/><stop offset="1" stop-color="#ff5a3c" stop-opacity="0"/></radialGradient></defs>';
  /* thành phố đổ nát xa xa */
  s+='<path id="skyline" d="M-440 70 L-440 -10 L-410 -10 L-410 -60 L-380 -60 L-380 -30 L-360 -30 L-355 -95 L-330 -100 L-322 -40 L-300 -40 L-300 -120 L-270 -128 L-262 -70 L-240 -70 L-240 -20 L-210 -20 L-210 70 Z M250 70 L250 -30 L280 -30 L284 -110 L300 -104 L312 -60 L330 -60 L330 -140 L352 -150 L360 -142 L364 -80 L390 -80 L392 -40 L420 -40 L420 -100 L446 -96 L446 70 Z" fill="#5b4638" opacity=".35"/>';
  s+='<g id="sun"><circle cx="360" cy="-235" r="30" fill="#f2c27a" opacity=".85"/><circle cx="360" cy="-235" r="46" fill="#f2c27a" opacity=".18"/></g>';
  s+='<g id="moon" opacity="0"><circle cx="360" cy="-235" r="60" fill="url(#moonG)"/><circle cx="360" cy="-235" r="26" fill="#d9573e"/><circle cx="352" cy="-242" r="5" fill="#b8432e" opacity=".7"/><circle cx="368" cy="-226" r="3.5" fill="#b8432e" opacity=".7"/></g>';
  s+='<g id="clouds"><g class="cloud"><ellipse cx="-330" cy="-235" rx="70" ry="18" fill="#7d6a5c" opacity=".55"/><ellipse cx="-296" cy="-250" rx="40" ry="18" fill="#8a7666" opacity=".5"/></g>'+
    '<g class="cloud"><ellipse cx="90" cy="-268" rx="80" ry="16" fill="#7d6a5c" opacity=".5"/><ellipse cx="124" cy="-282" rx="44" ry="16" fill="#8a7666" opacity=".45"/></g></g>';
  s+='<g id="world" style="transition:filter 1.2s">';
  /* đế mô hình: đất khô cằn */
  s+=box(-4,-3,-1,19,15.5,1,'#8b8f57','#f1efe9','#dcd8cf');
  s+=pg(qZ(0.005,-1.5,3.5,8.5,11),'#8a7356')+pg(qZ(0.005,10.5,14,7,10.5),'#8f785a')+pg(qZ(0.005,-3,1,-2,1.5),'#8a7356');
  [[[-2,4],[-0.5,5],[0.4,4.6]],[[11,8.5],[12.2,9.6],[13.4,9.2]],[[2,9.5],[3,10.4]]].forEach(function(c){s+='<polyline points="'+ps(c.map(function(p){return [p[0],p[1],0.01]}))+'" stroke="#5e5235" stroke-width="1.4" fill="none"/>'});
  /* zombie phía sau hàng rào */
  s+='<g id="zBehind">'+zombie('z3')+zombie('z4')+'</g>';
  s+=fenceLine([FX0,FY0],[FX1,FY0])+fenceLine([FX0,FY0],[FX0,FY1]);
  /* lối đi */
  for(var k=0;k<4;k++)s+=pg(qZ(0.01,6.05,7.35,7.2+k*1.15,7.95+k*1.15),'#b5a68c');
  s+=deadTree(12.8,1.7,2.0);
  /* ===== NHÀ THÔNG MINH HIỆN ĐẠI: khối hộp trắng 2 tầng (như mô hình formex) ===== */
  var WL='#f7f7f4',WR='#e2e3df',WT='#ececE8',GL='#4a6b80',FR='#2f353b';
  function glass(face,a0,a1,z0,z1,id,refl){
    var q=face==='y'?function(u0,u1,v0,v1,d){return qY(DP+d,u0,u1,v0,v1)}:face==='u'?function(u0,u1,v0,v1,d){return qY(5.0+d,u0,u1,v0,v1)}:
      face==='x'?function(u0,u1,v0,v1,d){return qX(HWID+d,u0,u1,v0,v1)}:function(u0,u1,v0,v1,d){return qX(6.4+d,u0,u1,v0,v1)};
    var r=pg(q(a0-0.1,a1+0.1,z0-0.1,z1+0.1,0.01),FR)+pg(q(a0,a1,z0,z1,0.02),GL,' id="'+id+'"');
    if(refl){var w=(a1-a0),pts=(face==='y'||face==='u')?[[a0+w*0.18,0,z1],[a0+w*0.32,0,z1],[a0+w*0.16,0,z0],[a0+w*0.02,0,z0]]:[[0,a0+w*0.18,z1],[0,a0+w*0.32,z1],[0,a0+w*0.16,z0],[0,a0+w*0.02,z0]];
      var Y0=face==='y'?DP+0.03:face==='u'?5.03:0,X0=face==='x'?HWID+0.03:face==='r'?6.43:0;
      r+=pg(pts.map(function(p){return face==='y'||face==='u'?[p[0],Y0,p[2]]:[X0,p[1],p[2]]}),'rgba(255,255,255,.2)')}
    return r;
  }
  s+=box(0,0,0,HWID,DP,3.3,WT,WL,WR);
  s+=box(-0.15,-0.15,3.3,HWID+0.3,DP+0.3,0.18,'#f4f4f1','#d6d7d3','#cbccc8');
  /* tầng 1: kính lớn, cửa sổ dải, ốp gỗ quanh cửa thông minh */
  s+=glass('y',0.5,4.9,0.55,2.85,'winF0',true)+pg(qY(DP+0.03,2.66,2.74,0.55,2.85),FR);
  s+=pg(qY(DP+0.01,5.7,9.7,0,3.3),'#c99566');
  for(var sl=5.95;sl<9.7;sl+=0.26)s+='<polyline points="'+ps([[sl,DP+0.015,0],[sl,DP+0.015,3.3]])+'" stroke="#b07d50" stroke-width=".9" fill="none"/>';
  s+='<text transform="'+mY([7.72,DP+0.03,2.95])+'" font-size="7" font-weight="800" letter-spacing="1.5" fill="#ffffff" font-family="system-ui,sans-serif">CERBERUS</text>';
  s+=pg(qY(DP+0.02,5.85,7.55,0,2.65),FR);
  s+=pg(qY(DP+0.03,6.0,7.4,0,2.5),'#20262b',' id="doorHole"');
  s+='<polygon id="leafOut" fill="#3d444b" stroke="#262b30" stroke-width="1"/><polyline id="leafBar" fill="none" stroke="#c9ccd0" stroke-width="2.4" stroke-linecap="round"/>';
  s+=pg(qY(DP+0.03,7.72,8.38,1.42,1.88),'#0b0f14')+pg(qY(DP+0.04,7.84,7.98,1.58,1.74),'#8ff0f8',' id="eyeL"')+pg(qY(DP+0.04,8.12,8.26,1.58,1.74),'#8ff0f8',' id="eyeR"');
  s+=pg(qY(DP+0.03,7.75,8.35,0.95,1.3),'#1e6b52');
  s+=box(5.6,DP,2.78,2.3,0.75,0.12,'#f4f4f1','#d6d7d3','#cbccc8');
  s+=box(5.7,DP,0,2.0,0.6,0.12,'#e9e9e5','#d2d3cf','#c4c5c1');
  s+=glass('x',0.8,5.6,1.5,2.7,'winS0',true)+pg(qX(HWID+0.03,2.37,2.43,1.5,2.7),FR)+pg(qX(HWID+0.03,3.97,4.03,1.5,2.7),FR);
  /* bảng nhiệt độ điện tử + camera an ninh ở góc */
  s+=pg(qX(HWID+0.02,5.85,6.35,1.0,1.42),'#15191d');
  s+='<text id="thermo" transform="'+mX([HWID+0.03,6.28,1.1])+'" font-size="8" font-weight="700" fill="#5ff2ff" font-family="monospace">28°</text>';
  /* tầng 2 + sân thượng kính */
  s+=box(0,0,3.48,6.4,5.0,2.6,WT,WL,WR);
  s+=glass('u',0.5,5.9,3.95,5.75,'winF1',true)+pg(qY(5.03,3.16,3.24,3.95,5.75),FR);
  s+=glass('r',0.8,4.2,4.1,5.6,'winS1',false);
  s+=box(-0.15,-0.15,6.08,6.7,5.3,0.2,'#f4f4f1','#d6d7d3','#cbccc8');
  /* pin mặt trời, ống thông gió, ăng-ten trên mái */
  [[0.5,2.0],[2.3,3.8],[4.1,5.6]].forEach(function(r){s+=box(r[0],0.6,6.28,1.5,3.6,0.08,'#24324a','#1b2638','#1b2638');
    for(var g=1;g<3;g++)s+='<polyline points="'+ps([[r[0]+g*0.5,0.6,6.37],[r[0]+g*0.5,4.2,6.37]])+'" stroke="#5f7fa8" stroke-width=".7" fill="none"/>';
    s+='<polyline points="'+ps([[r[0],2.4,6.37],[r[0]+1.5,2.4,6.37]])+'" stroke="#5f7fa8" stroke-width=".7" fill="none"/>'});
  s+=box(5.85,3.6,6.28,0.4,0.4,0.45,'#d6d7d3','#e4e4e0','#c8c9c5');
  s+='<g class="smoke" id="smoke" opacity="0" transform="translate('+P(6.05,3.8,6.9).join(' ')+')"><circle r="8"/><circle r="8"/><circle r="8"/></g>';
  s+='<polyline points="'+ps([[0.35,0.35,6.28],[0.35,0.35,8.0]])+'" stroke="#8c9196" stroke-width="2" fill="none"/><polyline points="'+ps([[0.0,0.35,7.6],[0.7,0.35,7.6]])+'" stroke="#8c9196" stroke-width="1.6" fill="none"/><polyline points="'+ps([[0.1,0.35,7.2],[0.6,0.35,7.2]])+'" stroke="#8c9196" stroke-width="1.6" fill="none"/>';
  /* lan can kính sân thượng */
  s+=pg(qY(DP+0.1,-0.1,HWID+0.1,3.48,4.15),'rgba(170,220,235,.35)',' stroke="rgba(255,255,255,.6)" stroke-width=".8"')+pg(qX(HWID+0.1,-0.1,DP+0.1,3.48,4.15),'rgba(150,200,220,.3)',' stroke="rgba(255,255,255,.6)" stroke-width=".8"');
  s+='<polyline points="'+ps([[-0.1,DP+0.1,4.15],[HWID+0.1,DP+0.1,4.15],[HWID+0.1,-0.1,4.15]])+'" stroke="#c4c8cc" stroke-width="2" fill="none"/>';
  s+=box(9.8,DP-0.1,2.95,0.28,0.28,0.2,'#ffffff','#e8e8e4','#d0d0cc')+icirc([9.98,DP+0.2,3.02],2.2,'#111')+icirc([10.05,DP+0.12,3.15],1.3,'#ff3b30',' id="camLed"');
  /* bồn cây trắng + bộ lưu điện năng lượng mặt trời */
  [[0.3,2.1],[2.5,4.3]].forEach(function(r){s+=box(r[0],DP+0.35,0,r[1]-r[0],0.5,0.45,'#f2f2ef','#ffffff','#e2e2de');for(var x=r[0]+0.25;x<r[1];x+=0.45)s+=icirc([x,DP+0.6,0.6],8,'#4f9a3f')+icirc([x+0.1,DP+0.55,0.75],5,'#62b04f')});
  s+=box(10.45,1.0,0,0.45,1.8,0.45,'#f2f2ef','#ffffff','#e2e2de');for(var yy2=1.2;yy2<2.8;yy2+=0.45)s+=icirc([10.67,yy2,0.6],7,'#4f9a3f');
  s+=box(10.45,4.3,0,0.8,1.1,0.95,'#f4f4f1','#e6e6e2','#d6d6d2')+pg(qX(11.26,4.5,5.2,0.6,0.7),'#59e3ff',' id="upsLed"');
  /* bao cát hai bên cổng, cột đèn chập chờn, cây chết */
  s+=sandbags(4.2,5.9,11.2)+sandbags(7.5,9.2,11.2);
  s+=box(8.6,10.0,0,0.14,0.14,3.0,'#2f3b46','#2f3b46','#26313a');
  s+=box(8.42,9.82,3.0,0.5,0.5,0.32,'#2f3b46','#3a4855','#2f3b46');
  s+=icirc([8.67,10.07,2.92],5,'#c9bf9a',' id="lampBulb"');
  s+=box(-2.4,8.3,0,0.26,0.26,1.3,'#7a5235','#6b4529','#5a3a22')+icirc([-2.27,8.43,2.0],20,'#4f9a3f')+icirc([-2.5,8.7,2.4],14,'#62b04f');
  /* zombie đập cửa (chỉ hiện khi cảm biến rung báo động) */
  s+='<g id="zDoorWrap" opacity="0">'+zombie('zDoor')+'</g>';
  /* hàng rào phía trước (có cổng) + biển báo */
  s+=fenceLine([FX1,FY0],[FX1,FY1])+fenceLine([FX0,FY1],[FX1,FY1],[5.9,7.5]);
  s+=pg([[5.95,FY1,0.15],[7.45,FY1,1.1],[7.45,FY1,1.3],[5.95,FY1,0.35]],'#7a573b')+pg([[5.95,FY1,1.1],[7.45,FY1,0.15],[7.45,FY1,0.35],[5.95,FY1,1.3]],'#6f5038');
  s+=pg(qY(FY1+0.02,1.4,5.0,0.55,1.3),'#f2c230')+pg(qY(FY1+0.03,1.4,5.0,0.55,0.63),'#1d1d1d')+pg(qY(FY1+0.03,1.4,5.0,1.22,1.3),'#1d1d1d');
  s+='<text transform="'+mY([1.6,FY1+0.03,0.83])+'" font-size="9" font-weight="800" fill="#1d1d1d" font-family="system-ui,sans-serif">⚠ VÙNG AN TOÀN</text>';
  s+='<g id="zFront">'+zombie('z0')+zombie('z1')+zombie('z2')+'</g>';
  s+='</g>';
  /* lớp phát sáng */
  s+='<g id="glowOut">';
  s+='<circle id="lampGlow" cx="'+P(8.67,10.07,2.9)[0]+'" cy="'+P(8.67,10.07,2.9)[1]+'" r="70" fill="url(#lampG)" opacity="0"/>';
  s+=pg(qY(DP+0.05,0.5,4.9,0.55,2.85),'#ffd27a',' id="gF0" opacity="0"')+pg(qY(5.05,0.5,5.9,3.95,5.75),'#ffd27a',' id="gF1" opacity="0"');
  s+=pg(qX(HWID+0.05,0.8,5.6,1.5,2.7),'#ffd27a',' id="gS0" opacity="0"')+pg(qX(6.45,0.8,4.2,4.1,5.6),'#ffd27a',' id="gS1" opacity="0"');
  s+='<g id="ledStrip" opacity=".3" style="transition:opacity 1s"><polyline points="'+ps([[-0.15,DP+0.16,3.3],[HWID+0.16,DP+0.16,3.3],[HWID+0.16,-0.15,3.3]])+'" stroke="#59e3ff" stroke-width="2" fill="none"/><polyline points="'+ps([[-0.15,5.16,6.08],[6.56,5.16,6.08],[6.56,-0.15,6.08]])+'" stroke="#59e3ff" stroke-width="2" fill="none"/></g>';
  s+=pg(qY(DP+0.03,6.0,7.4,0,2.5),'#ffcf7a',' id="gDoor" opacity="0"');
  s+='<circle id="fireOut" cx="'+P(HWID,3.2,2.1)[0]+'" cy="'+P(HWID,3.2,2.1)[1]+'" r="80" fill="url(#fireG)" opacity="0"/>';
  s+=icirc([0.35,0.35,8.05],2.6,'#ff3b30',' id="antLed"');
  s+='<g id="zEyes" opacity="0"></g>';
  s+='</g>';
  s+='<g id="planks"></g>';
  /* sương mù sát đất */
  s+='<g id="fog" style="pointer-events:none"><ellipse class="fogA" cx="-150" cy="250" rx="260" ry="40" fill="#cfc6b8" opacity=".22"/><ellipse class="fogB" cx="200" cy="300" rx="280" ry="44" fill="#cfc6b8" opacity=".2"/><ellipse class="fogA" cx="60" cy="120" rx="200" ry="30" fill="#cfc6b8" opacity=".12"/></g>';
  s+='<g id="rainOut" opacity="0"><g class="rain">';
  for(k=0;k<80;k++){var rx=-450+((k*137)%920),ry=-320+((k*251)%760);s+='<line x1="'+rx+'" y1="'+ry+'" x2="'+(rx-6)+'" y2="'+(ry+17)+'"/>'}
  s+='</g></g>';
  s+='<g id="groan" opacity="0" style="transition:opacity .4s"><rect x="-30" y="-20" width="60" height="18" rx="9" fill="rgba(20,16,12,.8)"/><text x="0" y="-7" text-anchor="middle" font-size="10" font-weight="700" fill="#b6e3a0" font-family="system-ui,sans-serif">Grrr...</text></g>';
  var dq=[[5.6,DP+0.2,0],[7.8,DP+0.2,0],[7.8,DP+0.2,3.0],[5.6,DP+0.2,3.0]];
  s+='<g class="hs" data-c="door" tabindex="0" role="button" aria-label="Đến cửa thông minh"><polygon points="'+ps(dq)+'" fill="transparent"/><polygon class="hl" points="'+ps(qY(DP+0.03,5.85,7.55,0,2.65))+'"/></g>';
  var pin=P(6.7,DP+0.9,3.4);
  s+='<g class="pin" style="pointer-events:none"><g transform="translate('+pin[0].toFixed(1)+' '+pin[1].toFixed(1)+') scale(1.25)"><rect x="-58" y="-34" width="116" height="26" rx="13" fill="#0b8a99"/><text x="0" y="-16.5" text-anchor="middle" font-size="12" font-weight="700" fill="#fff" font-family="system-ui,sans-serif">🚪 Cửa thông minh</text><path d="M-6 -8 L6 -8 L0 0 Z" fill="#0b8a99"/></g></g>';
  var hp=P(3.2,2.5,8.9);
  s+='<g id="houseTag" transform="translate('+hp[0].toFixed(1)+' '+hp[1].toFixed(1)+') scale(1.25)"><rect x="-70" y="-26" width="140" height="24" rx="12" fill="rgba(15,23,32,.78)"/><text id="houseTagT" x="0" y="-9.5" text-anchor="middle" font-size="12" font-weight="600" fill="#fff" font-family="system-ui,sans-serif">🏠 Ở nhà · 28°C</text></g>';
  $('#outSvg').innerHTML=s;
  var ey='';ZOMBIES.concat([{id:'zDoor'}]).forEach(function(z){ey+='<g id="'+z.id+'e"><circle cx="0.6" cy="-27.4" r="1.4" fill="#ff3b30"/><circle cx="4" cy="-27.6" r="1.4" fill="#ff3b30"/></g>'});
  $('#zEyes').innerHTML=ey;
}
function updZombies(now){
  var night=!!(H&&H.dark);setA('zEyes','opacity',night?1:0);
  ZOMBIES.forEach(function(z,i){
    var ph=((now*z.sp+z.ph)%2+2)%2,t=ph<1?ph:2-ph,dir=ph<1?1:-1;                /* đi tới rồi quay lại */
    var x=z.a[0]+(z.b[0]-z.a[0])*t,y=z.a[1]+(z.b[1]-z.a[1])*t,q=P(x,y,0);
    var sx=(z.b[0]-z.a[0])-(z.b[1]-z.a[1])>0?dir:-dir,bob=Math.abs(Math.sin(now/260+i))*1.6;
    var tr='translate('+q[0].toFixed(1)+' '+(q[1]-bob).toFixed(1)+') scale('+(1.5*sx)+' 1.5)';
    setA(z.id,'transform',tr);setA(z.id+'e','transform',tr);
    var g=document.getElementById(z.id);if(g){var l1=g.querySelector('.zleg1'),l2=g.querySelector('.zleg2'),sw=Math.sin(now/220+i)*14;
      l1.setAttribute('transform','rotate('+sw.toFixed(1)+' -2.5 -10)');l2.setAttribute('transform','rotate('+(-sw).toFixed(1)+' 2.5 -10)')}
  });
  var alarm=S.mode==='ALARM';setA('zDoorWrap','opacity',alarm?1:0);
  var zd=P(6.7,DP+0.75,0),hit=Math.abs(Math.sin(now/120));
  var trd='translate('+zd[0].toFixed(1)+' '+zd[1].toFixed(1)+') scale(-1.5 1.5)';setA('zDoor','transform',trd);setA('zDoore','transform',trd);setA('zDoore','opacity',alarm?1:0);
  var arm=document.querySelector('#zDoor .zarm');if(arm)arm.setAttribute('transform','rotate('+(-30*hit).toFixed(1)+' 0 -20)');
  /* thỉnh thoảng một con kêu "Grrr..." */
  var gi=Math.floor(now/6000)%ZOMBIES.length,on=(now%6000)<1600||alarm,gz=alarm?'zDoor':ZOMBIES[gi].id,el=document.getElementById(gz);
  if(el){var m=/translate\(([-\d.]+) ([-\d.]+)\)/.exec(el.getAttribute('transform')||'');if(m)setA('groan','transform','translate('+m[1]+' '+(+m[2]-52)+')')}
  setA('groan','opacity',on?1:0);
}

/* ---------- TRONG NHÀ: phòng rộng, LCD là trọng tâm, NeoPixel là đèn trần ---------- */
var DOOR_Y0=6.95,DOOR_Y1=8.55,DOOR_H=3.3,HALL=[2.6,7.75];
var RX=13,RY=9,RH=4.5,LCD_X0=4.15,LCD_X1=8.85,LCD_Z1=3.38,RING_C=[6.4,4.6,4.0],RING_R=0.95;
function ringPoly(r,z){var a=[];for(var k=0;k<28;k++){var an=k/28*Math.PI*2;a.push([RING_C[0]+Math.cos(an)*r,RING_C[1]+Math.sin(an)*r,z])}return a}
var COMP={
  lcd:{n:'Màn hình LCD1602',a:[6.5,0,3.62],poly:qY(0.02,3.95,9.05,2.1,3.62)},
  keypad:{n:'Bàn phím 6 nút',a:[9.95,0,1.35],poly:qY(0.02,9.4,10.5,1.35,2.3)},
  leds:{n:'3 đèn LED',a:[9.95,0,2.9],poly:qY(0.02,9.4,10.5,2.5,2.9)},
  hall:{n:'Đèn hành lang (LED vàng)',a:[HALL[0],HALL[1],3.65],poly:qY(HALL[1],HALL[0]-0.45,HALL[0]+0.45,2.95,3.9)},
  door:{n:'Cửa ra vào (servo)',a:[0,(DOOR_Y0+DOOR_Y1)/2,DOOR_H+0.8],poly:qX(0.02,DOOR_Y0-0.15,DOOR_Y1+0.15,0,DOOR_H+0.8)},
  ring:{n:'Đèn trần NeoPixel',a:[RING_C[0],RING_C[1],4.3],poly:ringPoly(1.2,RING_C[2])},
  dht:{n:'DHT11',a:[2.9,0,3.05],poly:qY(0.02,2.55,3.25,2.25,3.05)},
  buzzer:{n:'Còi chip',a:[2.9,0,3.9],poly:qY(0.02,2.55,3.25,3.2,3.9)},
  light:{n:'Cảm biến ánh sáng',a:[12.25,0.2,1.6],poly:[[11.9,0,1.45],[12.6,0,1.45],[12.6,0.45,1.45],[11.9,0.45,1.45],[11.9,0.45,1.9],[11.9,0,1.9]]},
  rain:{n:'Cảm biến mưa',a:[11.4,0,1.12],poly:qY(0.02,10.95,11.9,1.1,1.5)},
  flame:{n:'Cảm biến lửa',a:[1.35,0,2.75],poly:qY(0.02,1.0,1.7,2.25,2.75)},
  sound:{n:'Cảm biến âm thanh',a:[6.95,0.45,0.95],poly:qZ(0.92,6.65,7.25,0.25,0.7)},
  uno:{n:'Arduino Uno',a:[5.1,0.6,0.95],poly:qZ(0.92,4.45,5.75,0.25,0.95)},
  rtc:{n:'DS1307',a:[6.15,0.5,0.95],poly:qZ(0.92,5.9,6.45,0.3,0.75)}
};
var TAGDX={lcd:[0,-40],keypad:[-44,40],leds:[60,-16],hall:[55,-75],door:[0,-58],ring:[-84,-14],dht:[-30,-66],buzzer:[50,-56],light:[44,-36],rain:[42,38],flame:[-82,-24],sound:[72,30],uno:[-62,30],rtc:[-30,52]};
function buildIn(){
  var s='<defs><radialGradient id="ringG"><stop offset="0" stop-color="#ffd27a" stop-opacity=".75"/><stop offset="1" stop-color="#ffd27a" stop-opacity="0"/></radialGradient>'+
    '<radialGradient id="warmG"><stop offset="0" stop-color="#ffd27a" stop-opacity=".8"/><stop offset="1" stop-color="#ffd27a" stop-opacity="0"/></radialGradient>'+
    '<radialGradient id="lcdG"><stop offset="0" stop-color="#5b8dff" stop-opacity=".55"/><stop offset="1" stop-color="#5b8dff" stop-opacity="0"/></radialGradient>'+
    '<radialGradient id="fireG2"><stop offset="0" stop-color="#ff7b2e" stop-opacity=".85"/><stop offset="1" stop-color="#ff7b2e" stop-opacity="0"/></radialGradient>'+
    '<clipPath id="winClip"><polygon points="'+ps(qY(0.03,10.95,12.75,1.6,3.25))+'"/></clipPath></defs>';
  s+='<g id="room">';
  s+=box(-0.3,-0.3,-0.3,RX+0.3,RY+0.3,0.3,'#c99a66','#f1efe9','#dcd8cf');
  for(var k=1;k<RY;k++)s+='<polyline points="'+ps([[0,k,0],[RX,k,0]])+'" stroke="#b88a58" stroke-width="1" fill="none"/>';
  s+=box(-0.3,-0.3,0,0.3,RY+0.3,RH,'#fafafa','#e0d6c4','#efe6d6');
  s+=box(0,-0.3,0,RX,0.3,RH,'#fafafa','#f6eee0','#e0d6c4');
  s+=pg(qX(0.01,0,RY,0,0.25),'#d8ccb6')+pg(qY(0.01,0,RX,0,0.25),'#d8ccb6');
  /* góc bếp + cảm biến lửa */
  s+=box(0.3,0.15,0,1.9,1.35,1.4,'#e7e9ec','#d4d8dd','#c3c8ce');
  s+='<ellipse cx="'+P(0.8,0.85,1.4)[0]+'" cy="'+P(0.8,0.85,1.4)[1]+'" rx="9" ry="5" fill="#2c3036"/><ellipse cx="'+P(1.6,0.85,1.4)[0]+'" cy="'+P(1.6,0.85,1.4)[1]+'" rx="9" ry="5" fill="#2c3036"/>';
  s+=pg(qY(0.02,1.05,1.65,2.3,2.7),'#1f5fbf')+icirc([1.2,0.03,2.5],3,'#111')+icirc([1.5,0.03,2.5],2.4,'#3a7bd5');
  /* DHT11 + còi */
  s+=pg(qY(0.02,2.6,3.2,2.3,3.0),'#2f80ed')+pg(qY(0.03,2.7,3.1,2.42,2.9),'#5aa0ff');
  for(k=0;k<3;k++)s+=pg(qY(0.035,2.7,3.1,2.5+k*0.13,2.54+k*0.13),'#2f80ed');
  s+=icirc([2.9,0.02,3.5],8,'#151515')+icirc([2.9,0.03,3.5],2.5,'#444');
  /* LCD: trọng tâm của phòng (module xanh lá, màn xanh dương) */
  s+='<ellipse id="lcdGlow" cx="'+P(6.5,0.03,2.85)[0]+'" cy="'+P(6.5,0.03,2.85)[1]+'" rx="150" ry="90" fill="url(#lcdG)" opacity="0"/>';
  s+=pg(qY(0.02,3.95,9.05,2.1,3.62),'#1e7a4d');
  [[4.05,3.52],[8.95,3.52],[4.05,2.2],[8.95,2.2]].forEach(function(p){s+=icirc([p[0],0.025,p[1]],2.2,'#c9b77a')});
  s+=pg(qY(0.025,LCD_X0-0.08,LCD_X1+0.08,2.28,LCD_Z1+0.08),'#111a12');
  s+='<text transform="'+mY([4.2,0.03,2.16])+'" font-size="6" fill="#cfe9d6" font-family="monospace">LCD1602 I2C · 0x27</text>';
  /* tủ điều khiển dưới LCD: Arduino, DS1307, cảm biến âm thanh + dây nối lên LCD */
  s+=box(4.2,0.12,0,4.6,0.95,0.86,'#7a5a3e','#6a4d34','#5e442e');
  s+=pg(qY(0.02,4.6,4.66,0.92,2.1),'#d64545')+pg(qY(0.02,4.72,4.78,0.92,2.1),'#222')+pg(qY(0.02,4.84,4.9,0.92,2.1),'#e8c547')+pg(qY(0.02,4.96,5.02,0.92,2.1),'#3aa55c');
  s+=box(4.5,0.3,0.86,1.2,0.62,0.06,'#1b6fb8','#155a96','#165f9f')+pg(qZ(0.93,4.62,5.05,0.4,0.66),'#e9eef3')+pg(qZ(0.93,5.2,5.5,0.7,0.86),'#111');
  s+=box(5.92,0.33,0.86,0.48,0.38,0.06,'#2e7d32','#25692a','#276e2c')+icirc([6.15,0.52,0.94],2.6,'#c0c0c0');
  s+=box(6.68,0.28,0.86,0.52,0.4,0.06,'#1f5fbf','#184a96','#1a52a6')+icirc([6.95,0.48,0.96],3.2,'#2b2b2b',' id="micDot"');
  /* bàn phím + LED cạnh LCD */
  s+=pg(qY(0.02,9.4,10.5,1.35,2.3),'#22272e');
  s+=pg(qY(0.02,9.4,10.5,2.5,2.9),'#e9e2d3');
  /* cửa sổ + cảm biến mưa, ánh sáng */
  s+=pg(qY(0.01,10.8,12.9,1.45,3.4),'#ffffff');
  s+=pg(qY(0.02,10.95,12.75,1.6,3.25),'#a8d8f5',' id="winIn"');
  s+='<g id="rainIn" clip-path="url(#winClip)" opacity="0"><g class="rain">';
  for(k=0;k<14;k++){var a=P(10.95+(k%7)*0.3,0.03,3.6-(k>>3)*1.1);s+='<line x1="'+a[0]+'" y1="'+a[1]+'" x2="'+(a[0]-4)+'" y2="'+(a[1]+14)+'"/>'}
  s+='</g></g>';
  s+=pg(qY(0.03,11.81,11.89,1.6,3.25),'#ffffff')+pg(qY(0.03,10.95,12.75,2.38,2.46),'#ffffff');
  s+=box(10.75,0,1.42,2.2,0.45,0.06,'#ffffff','#ece6da','#ddd6c8');
  s+=pg(qY(0.02,11.0,11.85,1.12,1.42),'#1f5fbf')+'<g opacity=".7">'+[0,1,2].map(function(i){return '<polyline points="'+ps([[11.12+i*0.27,0.025,1.17],[11.12+i*0.27,0.025,1.37]])+'" stroke="#e6c35c" stroke-width="1.2" fill="none"/>'}).join('')+'</g>';
  s+=box(11.95,0.05,1.48,0.55,0.3,0.06,'#1f5fbf','#184a96','#1a52a6')+icirc([12.1,0.2,1.56],2.6,'#e07b39');
  /* tường trái: kệ sách + giường + đèn hành lang */
  s+=box(0,1.5,0,0.6,1.9,2.6,'#9a6c3e','#8a5f35','#a8784a');
  for(k=0;k<3;k++)for(var j=0;j<5;j++)s+=pg(qX(0.61,1.65+j*0.32,1.9+j*0.32,0.25+k*0.85,0.85+k*0.85),['#d9534f','#5bc0de','#f0ad4e','#5cb85c','#8e7cc3'][(j+k)%5]);
  s+=box(0,3.7,0,0.28,3.0,1.5,'#8a5f35','#7a5430','#9a6c3e');
  s+=box(0.28,3.7,0,3.9,3.0,0.5,'#9a6c3e','#8a5f35','#7a5430');
  s+=box(0.32,3.78,0.5,3.8,2.84,0.25,'#f4f1ea','#e4dfd4','#d8d2c5');
  s+=box(0.45,4.0,0.75,0.9,2.4,0.24,'#ffffff','#ece8e0','#e0dbd1');
  s+=box(1.55,3.72,0.5,2.62,2.96,0.36,'#e07a5f','#c9654b','#b85a42');
  s+=pg(qZ(0.87,1.7,4.0,4.2,6.2),'#e88f76');
  /* cửa ra vào trên tường trái: servo SG90 kéo cánh cửa, khi cháy ESP32 mở và giữ mở */
  s+=pg(qX(0.015,DOOR_Y0-0.15,DOOR_Y1+0.15,0,DOOR_H+0.15),'#6a4a30');
  s+=pg(qX(0.02,DOOR_Y0,DOOR_Y1,0,DOOR_H),'#2a1d18',' id="inHole"');
  s+=pg(qZ(0.012,0,2.3,DOOR_Y0,DOOR_Y1),'#ffd27a',' id="inDoorLight" opacity="0" style="mix-blend-mode:screen"');
  s+=pg(qX(0.02,DOOR_Y0-0.1,DOOR_Y1+0.1,DOOR_H+0.3,DOOR_H+0.78),'#14532d',' id="exitSign"');
  s+='<text transform="'+mX([0.03,DOOR_Y1-0.02,DOOR_H+0.42])+'" font-size="6.5" font-weight="700" fill="#eafff0" font-family="system-ui,sans-serif" id="exitTxt">THOÁT HIỂM</text>';
  s+=box(0.02,DOOR_Y0-0.62,DOOR_H-0.3,0.12,0.4,0.5,'#2c4f8f','#203c6e','#2a4a85');
  s+='<text transform="'+mX([0.16,DOOR_Y0-0.6,DOOR_H-0.05])+'" font-size="5" fill="#e6eefc" font-family="monospace">SG90</text>';
  s+='<polygon id="inLeaf" points="" fill="#8b5e3c" stroke="#5e3f27" stroke-width="1"/>';
  s+='<polygon id="inLeafPanel" points="" fill="none" stroke="#6e4a2e" stroke-width="1.2"/>';
  s+='<circle id="inKnob" r="2.6" fill="#e8e1d2" stroke="#5e3f27" stroke-width="1"/>';
  /* thảm, bàn trà, sofa quay về phía LCD, cây */
  s+=pg(qZ(0.01,4.5,9.8,1.6,5.4),'#7aa6c9')+pg(qZ(0.015,4.7,9.6,1.8,5.2),'#8db6d6');
  s+=box(5.7,1.9,0,1.8,0.8,0.4,'#d9b98f','#c4a074','#b99569');
  s+=box(4.9,3.3,0,3.4,1.1,0.55,'#5b8bd6','#4a77bf','#4370b6');
  s+=box(4.6,3.3,0,0.3,1.5,0.8,'#4f7fc7','#406db0','#3d68a8');
  s+=box(4.9,4.4,0,3.4,0.4,1.15,'#4f7fc7','#406db0','#3d68a8');
  s+=box(8.3,3.3,0,0.3,1.5,0.8,'#4f7fc7','#406db0','#3d68a8');
  /* vách hành lang ở lối vào: đèn vàng (D10) gắn ở đây, sáng khi có tiếng động người đi vào */
  s+=box(12.0,7.4,0,0.7,0.7,0.6,'#c96f4a','#b8603d','#a85636')+icirc([12.35,7.75,1.0],13,'#4f9a3f')+icirc([12.15,7.9,1.3],10,'#5daa4a');
  s+=pg(qZ(0.02,0.35,2.0,DOOR_Y0,DOOR_Y1),'#8b5e3c');
  s+='<text transform="'+mY([0.45,DOOR_Y1+0.45,0.02])+'" font-size="8" font-weight="700" fill="#a07a52" font-family="system-ui,sans-serif">LỐI VÀO</text>';
  /* đèn trần NeoPixel: dây treo + đế vòng */
  var top=P(RING_C[0],RING_C[1],6.2);
  [0,2.1,4.2].forEach(function(an){var p=P(RING_C[0]+Math.cos(an)*1.0,RING_C[1]+Math.sin(an)*1.0,RING_C[2]);s+='<line x1="'+top[0].toFixed(1)+'" y1="'+top[1].toFixed(1)+'" x2="'+p[0].toFixed(1)+'" y2="'+p[1].toFixed(1)+'" stroke="#6b6f75" stroke-width="1"/>'});
  s+='<circle cx="'+top[0].toFixed(1)+'" cy="'+top[1].toFixed(1)+'" r="3" fill="#6b6f75"/>';
  s+='</g>';
  var room=ps([[-0.3,RY,-0.3],[-0.3,RY,RH],[-0.3,-0.3,RH],[RX,-0.3,RH],[RX,-0.3,-0.3],[RX,RY,-0.3]]);
  s+='<polygon id="dim" points="'+room+'" fill="#070b18" opacity="0" style="transition:opacity .8s"/>';
  s+='<polygon id="tint" points="'+room+'" fill="#ffd27a" opacity="0" style="mix-blend-mode:soft-light"/>';
  /* lớp phát sáng */
  var fl=P(RING_C[0],RING_C[1],0);
  s+='<ellipse id="ringGlow" cx="'+fl[0].toFixed(1)+'" cy="'+fl[1].toFixed(1)+'" rx="150" ry="86" fill="url(#ringG)" opacity="0" style="mix-blend-mode:screen"/>';
  var o1=P(RING_C[0]-1.1,RING_C[1]+1.1,RING_C[2]),o2=P(RING_C[0]+1.1,RING_C[1]-1.1,RING_C[2]);
  s+='<polygon id="ringCone" points="'+o1[0].toFixed(1)+','+o1[1].toFixed(1)+' '+o2[0].toFixed(1)+','+o2[1].toFixed(1)+' '+(fl[0]+130).toFixed(1)+','+fl[1].toFixed(1)+' '+(fl[0]-130).toFixed(1)+','+fl[1].toFixed(1)+'" fill="#ffd27a" opacity="0" style="mix-blend-mode:screen"/>';
  s+='<path d="M'+ps(ringPoly(1.18,RING_C[2])).replace(/ /g,' L')+' Z M'+ps(ringPoly(0.7,RING_C[2]).reverse()).replace(/ /g,' L')+' Z" fill="#1d232a" fill-rule="evenodd"/>';
  for(k=0;k<12;k++){var a2=-Math.PI/2+k/12*Math.PI*2;s+=icirc([RING_C[0]+Math.cos(a2)*RING_R,RING_C[1]+Math.sin(a2)*RING_R,RING_C[2]],4.4,'#2a3138',' id="rd'+k+'" style="transition:fill .2s"')}
  s+='<image id="lcdImg" width="400" height="84" preserveAspectRatio="none" transform="'+lcdMatrix()+'"/>';
  var keyX=[9.5,9.85,10.2];
  for(k=0;k<6;k++){var kx=keyX[k%3],kz=k<3?2.2:1.8;s+=pg(qY(0.03,kx,kx+0.3,kz-0.3,kz),'#4a525c',' id="key'+(k+1)+'"');s+='<text transform="'+mY([kx+0.09,0.04,kz-0.08])+'" font-size="7.5" font-weight="700" fill="#fff" font-family="monospace">'+(k+1)+'</text>'}
  [['ledR',9.65,'#5a1f1f'],['ledG',9.95,'#1f4a2a'],['ledY',10.25,'#4f4520']].forEach(function(l){s+=icirc([l[1],0.03,2.7],4.5,l[2],' id="'+l[0]+'"')});
  var hl=P(HALL[0],HALL[1],0);var ht=P(HALL[0],HALL[1],6.2),hb=P(HALL[0],HALL[1],3.3);
  s+='<line x1="'+ht[0].toFixed(1)+'" y1="'+ht[1].toFixed(1)+'" x2="'+hb[0].toFixed(1)+'" y2="'+(hb[1]-6).toFixed(1)+'" stroke="#6b6f75" stroke-width="1"/><path d="M'+(hb[0]-7).toFixed(1)+' '+(hb[1]-1).toFixed(1)+' L'+(hb[0]+7).toFixed(1)+' '+(hb[1]-1).toFixed(1)+' L'+(hb[0]+4).toFixed(1)+' '+(hb[1]-8).toFixed(1)+' L'+(hb[0]-4).toFixed(1)+' '+(hb[1]-8).toFixed(1)+' Z" fill="#3b4148"/>';
  s+='<polygon id="hallCone" points="'+(hb[0]-5).toFixed(1)+','+hb[1].toFixed(1)+' '+(hb[0]+5).toFixed(1)+','+hb[1].toFixed(1)+' '+(hl[0]+40).toFixed(1)+','+hl[1].toFixed(1)+' '+(hl[0]-40).toFixed(1)+','+hl[1].toFixed(1)+'" fill="#ffd27a" opacity="0" style="mix-blend-mode:screen"/>';
  s+='<ellipse id="hallGlow" cx="'+hl[0].toFixed(1)+'" cy="'+hl[1].toFixed(1)+'" rx="62" ry="36" fill="url(#warmG)" opacity="0"/>';
  s+=icirc([HALL[0],HALL[1],3.25],4.5,'#d9d2c3',' id="hallBulb"');
  s+='<g class="buzz" id="buzzFx" transform="translate('+P(2.9,0.03,3.5).join(' ')+')"><circle r="3"/><circle r="3"/></g>';
  s+='<circle id="micRing" cx="'+P(6.95,0.48,0.96)[0]+'" cy="'+P(6.95,0.48,0.96)[1]+'" r="6" fill="none" stroke="#7fd3ff" stroke-width="2" opacity="0"/>';
  var fc=P(1.25,0.85,1.45);
  s+='<circle id="fireGlow" cx="'+fc[0].toFixed(1)+'" cy="'+fc[1].toFixed(1)+'" r="90" fill="url(#fireG2)" opacity="0"/>';
  s+='<g id="flames" opacity="0" transform="translate('+fc[0].toFixed(1)+' '+fc[1].toFixed(1)+')"><g class="flame"><path d="M-14 0 C-18 -14 -8 -20 -6 -30 C0 -20 6 -18 4 -8 C8 -12 10 -16 10 -20 C18 -8 12 4 -2 4 Z" fill="#ff7b2e"/><path d="M-6 0 C-8 -8 -2 -12 0 -18 C4 -10 6 -6 4 0 Z" fill="#ffd166"/></g>'+
    '<g class="smoke" transform="translate(-2 -30)"><circle r="7"/><circle r="7"/><circle r="7"/></g></g>';
  s+='<g id="hsLayer">';Object.keys(COMP).forEach(function(id){s+=hs(id,COMP[id].poly,COMP[id].n)});
  s+='</g><g id="tagLayer">';Object.keys(COMP).forEach(function(id){var d=TAGDX[id]||[0,-30];s+=tag(COMP[id].a,COMP[id].n,'opt',d[0],d[1])});
  s+='</g>';
  $('#inSvg').innerHTML=s;
}
function lcdMatrix(){var q=P(LCD_X0,0.03,LCD_Z1),k=(LCD_X1-LCD_X0)/400;return 'matrix('+(C30*IS*k)+' '+(S30*IS*k)+' 0 '+(IS*k)+' '+q[0].toFixed(1)+' '+q[1].toFixed(1)+')'}

/* ---------- LCD 16x2 vẽ lại từ dữ liệu Arduino gửi sang ---------- */
var lcdCv=document.createElement('canvas');lcdCv.width=400;lcdCv.height=84;var lcdCx=lcdCv.getContext('2d'),lcdKey='';
function lcdGlyph(code,cg){if(code<8)return cg[code]||[0,0,0,0,0,0,0,0];if(code===255)return [31,31,31,31,31,31,31,31];if(code===0xA5)return [0,0,0,14,14,0,0,0];if(code===0xDF)return [6,9,9,6,0,0,0,0];if(code>=32&&code<127)return FONT[code-32];return [0,0,0,0,0,0,0,0]}
function renderLcd(){
  var img=$('#lcdImg');if(!img)return;
  var l=H&&H.lcd,key=l?JSON.stringify([l.rows,l.cg,l.bl]):'off';
  if(key===lcdKey)return;lcdKey=key;
  var bl=l&&l.bl;lcdCx.fillStyle=!l?'#0a1220':bl?'#2b63c9':'#0d1f3d';lcdCx.fillRect(0,0,400,84);
  if(l)for(var r=0;r<2;r++)for(var c=0;c<16;c++){var g=lcdGlyph(l.rows[r][c],l.cg),x0=10+c*24,y0=8+r*38;
    for(var y=0;y<8;y++)for(var x=0;x<5;x++){var on=(g[y]>>(4-x))&1;lcdCx.fillStyle=on?(bl?'#eef6ff':'#5f7ba3'):(bl?'rgba(255,255,255,.07)':'rgba(255,255,255,.03)');lcdCx.fillRect(x0+x*4,y0+y*4,3,3)}}
  img.setAttribute('href',lcdCv.toDataURL());
}

/* ---------- cập nhật cảnh theo dữ liệu (mỗi khung hình) ---------- */
function rgbAvg(ring){var r=0,g=0,b=0;(ring||[]).forEach(function(c){r+=c[0];g+=c[1];b+=c[2]});var n=(ring&&ring.length)||1;return [r/n,g/n,b/n]}
function boost(c){var m=Math.max(c[0],c[1],c[2]);if(m<3)return null;var k=Math.pow(m/255,.55)*255/m;return 'rgb('+Math.min(255,c[0]*k|0)+','+Math.min(255,c[1]*k|0)+','+Math.min(255,c[2]*k|0)+')'}
function setA(id,attr,v){var e=document.getElementById(id);if(e&&e.getAttribute(attr)!==String(v))e.setAttribute(attr,v)}
function updOut(now){
  var h=H||{},dark=!!h.dark,rain=!!h.raining,fire=!!h.fire;
  var st=$('#stage');st.classList.toggle('night',dark);
  var fx=dark?'brightness(.42) saturate(.7) hue-rotate(-8deg)':'saturate(.85)';['world','planks'].forEach(function(id){var w=document.getElementById(id);if(w)w.style.filter=fx});
  setA('sun','opacity',dark?0:1);setA('moon','opacity',dark?1:0);
  $$('#clouds ellipse').forEach(function(e){e.setAttribute('fill',rain?'#4e4a48':'#7d6a5c')});updZombies(now);
  setA('rainOut','opacity',rain?1:0);
  var avg=rgbAvg(h.ring),lit=Math.max(avg[0],avg[1],avg[2]),rc=boost(avg)||'#ffd27a';
  ['gF0','gF1'].forEach(function(id){setA(id,'fill',fire?'#ff6a2b':rc);setA(id,'opacity',fire?(0.55+0.35*Math.abs(Math.sin(now/90))).toFixed(2):Math.min(.95,lit/160).toFixed(2))});
  setA('gS1','opacity',h.hall?.75:0);setA('gS1','fill','#ffd27a');
  setA('gS0','fill',fire?'#ff5a1f':rc);setA('gS0','opacity',fire?(0.6+0.35*Math.abs(Math.sin(now/70))).toFixed(2):Math.min(.8,lit/180).toFixed(2));
  setA('fireOut','opacity',fire?(0.6+0.3*Math.sin(now/120)).toFixed(2):0);
  setA('smoke','opacity',fire?1:0);
  var flick=dark&&((now%2300)<1900||(now%170)<80);setA('lampGlow','opacity',flick?1:0);setA('lampBulb','fill',flick?'#fff2c2':'#c9bf9a');
  setA('antLed','opacity',(now%1400)<180?1:0.15);
  ['winF0','winF1','winS0','winS1'].forEach(function(id){setA(id,'fill',dark?'#141b24':'#4a6b80')});
  setA('ledStrip','opacity',dark?0.95:0.3);setA('camLed','opacity',(now%1800)<900?1:0.25);setA('upsLed','opacity',0.6+0.4*Math.sin(now/600));
  /* cánh cửa quay theo góc servo */
  var a=S.open*Math.PI/2*0.92,x0=6.0,len=1.4,dx=Math.cos(a)*len,dy=Math.sin(a)*len;
  setA('leafOut','points',ps([[x0,DP+0.02,0],[x0+dx,DP+0.02+dy,0],[x0+dx,DP+0.02+dy,2.5],[x0,DP+0.02,2.5]]));
  setA('leafBar','points',ps([[x0+dx*0.86,DP+0.04+dy*0.86,0.95],[x0+dx*0.86,DP+0.04+dy*0.86,1.75]]));
  setA('gDoor','opacity',S.open>0.05?(dark?0.75:0.35)*S.open:0);
  var blink=(now%4200)<160;setA('eyeL','opacity',blink?0.15:1);setA('eyeR','opacity',blink?0.15:1);
  var t=h.dhtOk?Math.round(h.temp)+'°':'--';var th=document.getElementById('thermo');if(th&&th.textContent!==t)th.textContent=t;
  var label=!H?'🏠 Mất kết nối với nhà':fire?'🔥 CÓ LỬA TRONG NHÀ':h.intrude?'🚨 CÓ KẺ LẠ TRONG NHÀ':(h.mode===1?'🔒 Vắng nhà':h.mode===2?'🌙 Đang ngủ':'🏠 Ở nhà')+(h.dhtOk?' · '+Math.round(h.temp)+'°C':'')+(rain?' · mưa':'');
  var hT=document.getElementById('houseTagT');if(hT&&hT.textContent!==label){hT.textContent=label;var r=hT.previousSibling;var wd=Math.max(140,label.length*7.2+20);r.setAttribute('x',-wd/2);r.setAttribute('width',wd);r.setAttribute('fill',fire||h.intrude?'rgba(192,48,47,.9)':'rgba(15,23,32,.78)')}
}
function updIn(now){
  var h=H||{},dark=!!h.dark;
  $('#stage').classList.toggle('night',dark);
  var avg=rgbAvg(h.ring),lit=Math.max(avg[0],avg[1],avg[2]);
  (h.ring||[]).forEach(function(c,i){setA('rd'+i,'fill',boost(c)||'#2a3138')});
  if(!h.ring)for(var i=0;i<12;i++)setA('rd'+i,'fill','#2a3138');
  var rc=boost(avg);
  setA('ringGlow','opacity',rc?Math.min(.95,lit/120).toFixed(2):0);setA('ringCone','opacity',rc?Math.min(.18,lit/900).toFixed(3):0);if(rc){var stop=$('#ringG stop');if(stop)stop.setAttribute('stop-color',rc);setA('ringCone','fill',rc)}
  setA('lcdGlow','opacity',h.lcd&&h.lcd.bl?(dark?0.9:0.45):0);
  var dimv=dark?(lit>40||h.hall?0.28:0.55):0;setA('dim','opacity',dimv);
  setA('tint','opacity',rc&&dark?Math.min(.5,lit/300).toFixed(2):0);if(rc)setA('tint','fill',rc);
  setA('winIn','fill',dark?'#1b2a4a':'#a8d8f5');setA('rainIn','opacity',h.raining?1:0);
  var L=h.leds||{};setA('ledR','fill',L.r?'#ff4d4d':'#5a1f1f');setA('ledG','fill',L.g?'#3ee07a':'#1f4a2a');setA('ledY','fill',L.y?'#ffd84d':'#4f4520');
  setA('hallBulb','fill',h.hall?'#fff2b3':'#d9d2c3');setA('hallGlow','opacity',h.hall?1:0);setA('hallCone','opacity',h.hall?0.22:0);
  for(var k=1;k<=6;k++)setA('key'+k,'fill',h.key===k?'#0b8a99':'#4a525c');
  var bz=document.getElementById('buzzFx');if(bz)bz.classList.toggle('on',!!h.bz);
  setA('micRing','opacity',h.sound?1:0);setA('micDot','fill',h.sound?'#7fd3ff':'#2b2b2b');
  setA('fireGlow','opacity',h.fire?(0.65+0.3*Math.sin(now/110)).toFixed(2):0);setA('flames','opacity',h.fire?1:0);
  /* cánh cửa quay vào trong theo góc servo */
  var an=S.open*Math.PI/180*85,L=DOOR_Y1-DOOR_Y0,ex=0.03+Math.sin(an)*L,ey=DOOR_Y0+Math.cos(an)*L;
  setA('inLeaf','points',ps([[0.03,DOOR_Y0,0],[ex,ey,0],[ex,ey,DOOR_H],[0.03,DOOR_Y0,DOOR_H]]));
  var pa=function(t,z){return [0.03+Math.sin(an)*L*t,DOOR_Y0+Math.cos(an)*L*t,z]};
  setA('inLeafPanel','points',ps([pa(.18,.35),pa(.82,.35),pa(.82,DOOR_H-.35),pa(.18,DOOR_H-.35)]));
  var kq=P.apply(null,pa(.86,1.55));setA('inKnob','cx',kq[0].toFixed(1));setA('inKnob','cy',kq[1].toFixed(1));
  setA('inHole','fill',S.open>0.05?(dark?'#3a2a36':'#f5d9a6'):'#2a1d18');
  setA('inDoorLight','opacity',(S.open*(h.fire?0.55:0.35)).toFixed(2));
  var ex2=document.getElementById('exitSign');if(ex2){ex2.setAttribute('fill',h.fire?'#22c55e':'#14532d');ex2.classList.toggle('exitglow',!!h.fire)}
  renderLcd();
}

/* ---------- thẻ thông tin linh kiện ---------- */
var FX_NAME=['Đèn thường','Bình minh (báo thức)','Đếm ngược rời nhà','Chào mừng (vệt xanh chạy vòng)','Báo mưa (giọt xanh chạy vòng)','Báo nóng (đỏ cam thở nhịp)','Báo cháy (đỏ cam lập lòe)','Báo động cửa (đỏ nhấp nháy)'];
var LMODE=['Tự động theo trời','Luôn bật','Luôn tắt'];
function spark(arr){
  var v=arr.map(function(x,i){return [i,x]}).filter(function(p){return p[1]!==255});if(v.length<2)return '<div class="small" style="margin-top:8px">Biểu đồ 12 giờ: đang thu thập (mỗi giờ 1 điểm).</div>';
  var lo=Math.min.apply(null,v.map(function(p){return p[1]})),hi=Math.max.apply(null,v.map(function(p){return p[1]}));if(hi===lo)hi=lo+1;
  var pts=v.map(function(p){return (p[0]*(280/11)+4).toFixed(1)+','+(40-(p[1]-lo)*34/(hi-lo)).toFixed(1)}).join(' ');
  return '<svg class="spark" viewBox="0 0 288 46" preserveAspectRatio="none" aria-label="Nhiệt độ 12 giờ qua"><polyline points="'+pts+'" fill="none" stroke="var(--accent)" stroke-width="2"/></svg><div class="small">Nhiệt độ 12 giờ qua: '+lo+'–'+hi+'°C</div>';
}
function infoFor(id){
  var h=H;if(!h)return {t:COMP[id].n,sub:'',big:'Mất kết nối',p:'Arduino không gửi dữ liệu sang ESP32. Kiểm tra dây Serial D0/D1 ↔ G25/G26 và dây GND chung.'};
  var light=h.light,rows=[];
  switch(id){
    case 'lcd':return {t:'Màn hình LCD1602 I2C',sub:'SDA → A4 · SCL → A5 · địa chỉ 0x27',big:h.pageName,rows:[['Đèn nền',h.lcd&&h.lcd.bl?'Bật':'Tắt (tự tắt sau 30 giây không bấm)'],['Người vào gần nhất',h.lastEntry?h.lastEntry.name+' lúc '+h.lastEntry.time:'Chưa có']],p:'Hình trên web được vẽ lại từ nội dung Arduino gửi sang, nên giống hệt màn thật.'};
    case 'keypad':return {t:'Bàn phím 6 nút',sub:'Hàng D4, D5 · Cột D6, D7, A3',big:h.key?'Đang bấm phím '+h.key:'LCD: '+h.pageName,rows:[['Bấm · xem','1 Nhà · 2 Cửa · 3 Cảm biến · 4 Đèn · 5 Chế độ · 6 Báo thức'],['Bấm lại','1: bảng phím tự chạy · 3: từng cảm biến'],['Giữ 1 giây · làm','1 bảng phím · 2 mở cửa · 3 kiểm tra · 4 đổi đèn · 5 vắng nhà · 6 ngủ/dậy']],p:'Mỗi phím là một chủ đề của nhà. Bấm nhanh chỉ để xem, giữ 1 giây mới làm, buông sớm là hủy. Lúc khởi động và lúc màn tắt mà bấm phím, bảng phím tự chạy từng phím một. Trên web chỉ xem, không bấm được.'};
    case 'leds':return {t:'3 đèn LED trạng thái',sub:'Đỏ D8 · Xanh D9 · Vàng (hành lang) D10',big:[(h.leds.r?'🔴':'⚫'),(h.leds.g?'🟢':'⚫'),(h.leds.y?'🟡':'⚫')].join(' '),rows:[['Xanh','Sáng khi ở nhà'],['Đỏ','Nháy khi mưa, nóng hoặc cháy'],['Vàng','Đèn hành lang']]};
    case 'hall':return {t:'Đèn hành lang',sub:'LED vàng · D10 · treo trần ở lối vào',big:h.hall?'Đang bật':'Đang tắt',rows:[['Chế độ',LMODE[h.hallMode]],['Bật do cửa mở',h.doorLight?'Có (còn sáng)':'Không']],p:'Bóng đèn treo trần ngay trên thảm lối vào. Cửa mở lúc trời tối thì đèn tự sáng 30 giây soi đường. Vỗ tay 2 lần cũng bật được 5 giây. Xa giường nên không làm phiền giấc ngủ.'};
    case 'ring':return {t:'Đèn trần NeoPixel 12 LED',sub:'DIN → D3 · 5V · tụ 470µF ở chân nguồn',big:FX_NAME[h.ringFx]||'Đèn thường',rows:[['Đèn phòng khách',h.ringOn?'Bật':'Tắt'],['Chế độ',LMODE[h.lightMode]]]};
    case 'dht':return {t:'Cảm biến DHT11',sub:'DATA → D2',big:h.dhtOk?h.temp+'°C · '+h.hum+'%':'Lỗi đọc',rows:[['Hôm nay',h.dayMax>=0?h.dayMin+'–'+h.dayMax+'°C':'Đang thu thập'],['Báo nóng','Từ '+35+'°C']],extra:spark(h.hist12||[])};
    case 'buzzer':return {t:'Còi chip 5V',sub:'D11 nối thẳng · chân (−) về GND',big:h.bz?'Đang kêu':'Im lặng',p:'Kêu khi bấm phím, báo mưa, báo nóng, báo cháy và khi cửa báo động.'};
    case 'light':return {t:'Cảm biến ánh sáng',sub:'AO → A0',big:(h.dark?'Tối':'Sáng')+' ('+light+')',p:'Số càng lớn càng tối. Trên 460 là tối, dưới 340 là sáng (có vùng đệm để đèn không chớp).'};
    case 'rain':return {t:'Cảm biến mưa',sub:'AO → A1',big:(h.raining?'Đang mưa':'Khô ráo')+' ('+h.rain+')',rows:[['Số lần mưa hôm nay',h.rainCount]],p:'Dưới 500 là ướt. Có mưa 2 giây mới báo, hết mưa 5 giây mới tắt.'};
    case 'flame':return {t:'Cảm biến lửa',sub:'AO → A2',big:(h.fire?'CÓ LỬA':'Không có lửa')+' ('+h.flame+')',rows:[['Số lần cháy',h.fireCount]],p:'Dưới 400 trong 1 giây là báo cháy, trên 520 trong 5 giây là hết. Có cháy thì ESP32 nhắn Telegram ngay.'};
    case 'sound':return {t:'Cảm biến âm thanh',sub:'DO → D12',big:h.sound?'Có tiếng động':'Yên lặng',rows:[['Vỗ tay hôm nay',h.clap]],p:'Ở nhà: vỗ tay 2 lần bật đèn hành lang. Vắng nhà đang canh gác: có tiếng động mà không ai quẹt thẻ là báo KẺ LẠ, còi kêu và ESP32 nhắn Telegram. Về nhà thì quẹt thẻ ở cửa.'};
    case 'uno':return {t:'Arduino Uno R3',sub:'Nối ESP32: D1 → (chia áp) → G26 · D0 ← G25 · GND chung',big:homeModeName(h),rows:[['Kết nối với ESP32','Tốt · gửi trạng thái mỗi giây'],['Giờ',h.time]]};
    case 'rtc':return {t:'Đồng hồ DS1307',sub:'SDA → A4 · SCL → A5',big:h.time,p:'Giữ giờ cả khi mất điện nhờ pin CR2032. Dùng cho giờ đi ngủ 23h và báo thức 6h.'};
  }
}
function paintInfo(){
  var box=$('#info');if(!infoSel||VIEW!=='in'){box.hidden=true;return}
  var f=infoFor(infoSel);box.hidden=false;
  var html='<button class="x" type="button" data-scene="info-x" aria-label="Đóng">×</button><h3>'+esc(f.t)+'</h3><div class="sub">'+esc(f.sub||'')+'</div><div class="big">'+esc(f.big||'')+'</div>';
  if(f.rows)html+='<dl class="kv">'+f.rows.map(function(r){return '<dt>'+esc(r[0])+'</dt><dd>'+esc(r[1])+'</dd>'}).join('')+'</dl>';
  if(f.extra)html+=f.extra;
  if(f.p)html+='<p>'+esc(f.p)+'</p>';
  if(box.innerHTML!==html)box.innerHTML=html;
  $$('#hsLayer .hs').forEach(function(g){g.classList.toggle('sel',g.dataset.c===infoSel)});
}


/* ---------- robot hướng dẫn ---------- */
function guideMsg(){
  var h=H;
  if(h&&h.fire){
    if(S.mode==='FIRE'||S.door)return VIEW==='in'?'🔥 CHÁY! Cửa đã tự mở, đi theo biển THOÁT HIỂM ra ngoài ngay!':'🔥 Trong nhà có cháy! Cửa đã tự mở để mọi người thoát ra.';
    return h.mode===1?'🔥 Có lửa lúc vắng nhà! Cửa giữ đóng để tránh kẻ gian, mình đã báo Telegram.':'🔥 Có lửa trong nhà! Mình đã báo Telegram.';
  }
  if(h&&h.intrude)return '🚨 Có tiếng động trong nhà khi bạn vắng nhà! Mình đã nhắn Telegram. Quẹt thẻ ở cửa để tắt.';
  if(S.mode==='ALARM')return VIEW==='in'?'🚨 Có người va đập mạnh vào cửa!':'🚨 Zombie đang đập cửa! Cảm biến rung đã bật còi.';
  if(!h)return 'Mình không nghe được Arduino trong nhà. Kiểm tra dây Serial giữa 2 board nhé.';
  if(VIEW==='in'){
    if(h.mode===1)return h.armed?'Nhà đang canh gác: có tiếng động là báo kẻ lạ. Quẹt thẻ ở cửa để về nhà.':'Đang đếm ngược để rời nhà…';
    if(h.mode===2)return 'Cả nhà đang ngủ. Đèn chuyển tím mờ, 6 giờ sáng mình báo thức.';
    if(h.mode===3)return 'Chào mừng '+(h.whoName&&h.whoName!=='Chu nha'?h.whoName:'chủ nhà')+' về nhà! Mình đang kể lúc vắng nhà đã có gì.';
    if(h.doorLight)return 'Cửa vừa mở lúc tối, mình bật đèn hành lang 30 giây soi đường.';
    if(h.raining)return 'Ngoài trời đang mưa đó, thu đồ vào nhé!';
    if(h.dhtOk&&h.temp>=33)return 'Trong nhà '+h.temp+'°C, nóng quá! Uống nước nhé.';
    if(h.dark&&h.ringOn)return 'Trời tối rồi, mình đã bật đèn phòng khách.';
    return 'Trong nhà '+(h.dhtOk?h.temp+'°C, '+h.hum+'%':'')+'. Bấm vào linh kiện để xem chi tiết.';
  }
  if(Date.now()%24000<8000)return 'TRÙNG SINH TRƯỚC NGÀY TẬN THẾ ZOMBIE, TÔI XÂY DỰNG CĂN NHÀ TRÚ ẨN THÔNG MINH BẢO VỆ BẢN THÂN';
  if(h.raining)return 'Trời mưa, zombie vẫn lảng vảng ngoài hàng rào. Vào nhà thôi!';
  if(h.dark)return 'Đêm xuống, zombie mò ra nhiều hơn. Đèn trong nhà '+(h.ringOn?'đang sáng':'đang tắt')+'.';
  return 'Ngoài hàng rào toàn zombie! Bấm vào cửa, quẹt thẻ rồi vào nhà cho an toàn nhé.';
}


function canEnter(){return S.door||Date.now()-S.lastOpen<6000||VIEW==='in'}
var ORDER={out:0,door:1,in:2};
function go(v,force){
  if(v===VIEW)return;
  if(v==='in'&&!canEnter()&&!force){toast('Cửa đang đóng. Bấm Mở cửa trước nhé.');if(VIEW!=='door')go('door');return}
  var from=VIEW,deeper=ORDER[v]>ORDER[from],fe=$('#sc-'+from),te=$('#sc-'+v);
  if(from==='out'&&deeper){var dp=P(6.7,DP,1.2),svg=$('#outSvg').getBoundingClientRect(),vb=[-440,-300,900,720],sc=Math.min(svg.width/vb[2],svg.height/vb[3]);
    fe.style.transformOrigin=((svg.width-vb[2]*sc)/2+(dp[0]-vb[0])*sc)+'px '+((svg.height-vb[3]*sc)/2+(dp[1]-vb[1])*sc)+'px'}
  else fe.style.transformOrigin='50% 60%';
  fe.style.transform=deeper?'scale(2.4)':'scale(.9)';fe.dataset.on='0';
  te.style.transformOrigin='50% 60%';te.style.transition='none';te.style.transform=deeper?'scale(.92)':'scale(1.15)';
  void te.offsetWidth;te.style.transition='';te.style.transform='scale(1)';te.dataset.on='1';
  VIEW=v;var st=$('#stage');st.classList.toggle('door',v==='door');st.classList.toggle('inside',v==='in');
  $$('.crumbs [data-view]').forEach(function(b){if(b.dataset.view===v)b.setAttribute('aria-current','step');else b.removeAttribute('aria-current')});
  if(v!=='in'){infoSel=null;$('#info').hidden=true}
}
function sceneFrame(t){
  if(!sceneBuilt){buildOut();buildIn();sceneBuilt=true}
  if(t-sceneLastFr<33)return;var dt=Math.min(100,t-sceneLastFr);sceneLastFr=t;var now=Date.now();
  var target=Math.max(0,Math.min(1,(S.servo||0)/90));S.open+=(target-S.open)*Math.min(1,dt/120);
  $('#leaf').setAttribute('transform','translate(70 0) scale('+(1-.8*S.open).toFixed(3)+' 1) translate(-70 0)');$('#glow').setAttribute('opacity',(S.open*.5).toFixed(2));
  if(VIEW==='out')updOut(now);else if(VIEW==='in')updIn(now);else $('#stage').classList.remove('night');
  var ce=canEnter()&&VIEW==='door';$('#doorsvg').classList.toggle('can',ce);setA('enterTxt','opacity',ce&&S.open>0.4?1:0);$('#btnEnter').disabled=!canEnter();
  var src=$('#oled');[$('#oledScene'),$('#oledGuide')].forEach(function(c){var x=c.getContext('2d');x.imageSmoothingEnabled=false;x.drawImage(src,0,0)});
  $('#rcBox').classList.toggle('on',S.mode==='READY'||S.mode==='GRANT');
  var g=guideMsg();if(g!==lastGuide){lastGuide=g;$('#bubble').textContent=g}
  if(VIEW==='in'){paintInfo();drawLcd($('#lcdScene'),H&&H.lcd);var lp=$('#lcdScenePage'),pn=(H&&H.pageName)||'—';if(lp.textContent!==pn)lp.textContent=pn}
}
function sceneDoorText(){
  var M={IDLE:'Cửa đóng · đang canh',READY:'Có người trước cửa',GRANTED:'Đang mở cho khách',OWNER:'Chủ nhà về',GRANT:'Đang chờ thẻ mới',DENIED:'Thẻ lạ',LOCKED:'Đầu đọc khóa 30 giây',ALARM:'BÁO ĐỘNG',FIRE:'CHÁY · cửa mở thoát hiểm'};
  $('#dsSt').textContent=S.mode==='FIRE'?M.FIRE:S.door?'Cửa đang mở':(M[S.mode]||'Cửa đóng');
  $('#dsSub').textContent=S.door?'Bấm vào cánh cửa để vào nhà':'Quẹt thẻ ở đầu đọc, hoặc mở từ đây';
}
document.addEventListener('click',function(e){
  var v=e.target.closest('[data-view]');if(v&&!v.disabled){go(v.dataset.view);return}
  if(e.target.closest('#doorsvg')){if(canEnter())go('in');else toast(S.mode==='ALARM'?'Đang báo động, không mở được cửa!':'Cửa đang đóng. Bấm Mở cửa trước nhé.');return}
  var c=e.target.closest('.hs');if(c){if(c.dataset.c==='door'){go('door');return}infoSel=infoSel===c.dataset.c?null:c.dataset.c;paintInfo();if(!infoSel)$$('#hsLayer .hs').forEach(function(x){x.classList.remove('sel')});return}
  if(e.target.closest('[data-scene="info-x"]')){infoSel=null;paintInfo();$$('#hsLayer .hs').forEach(function(x){x.classList.remove('sel')});return}
  if(e.target.closest('#btnLabels')){var b=$('#btnLabels');labelsOn=!labelsOn;$('#inSvg').classList.toggle('labels-off',!labelsOn);b.textContent=labelsOn?'Ẩn tên linh kiện':'Hiện tên linh kiện';b.setAttribute('aria-pressed',String(labelsOn))}
});
document.addEventListener('keydown',function(e){
  if((e.key==='Enter'||e.key===' ')&&e.target.classList&&(e.target.classList.contains('hs')||e.target.id==='doorsvg')){e.preventDefault();e.target.dispatchEvent(new MouseEvent('click',{bubbles:true}))}
  if(e.key==='Escape'&&infoSel){infoSel=null;paintInfo()}
});

/* ================= TRẠNG THÁI TRANG ================= */
var PD=null,log=[],since=0,cardsVer=-1,cards=[],owner=null,filt='all';
var PAGES=[['tong-quan','Tổng quan','i-grid'],['ngoi-nha','Ngôi nhà','i-iso'],['cua','Cửa','i-door'],['trong-nha','Trong nhà','i-home'],['the','Thẻ','i-card'],['nhat-ky','Nhật ký','i-list']];
$('#nav').innerHTML=PAGES.map(function(p){return '<button type="button" data-go="'+p[0]+'"><svg><use href="#'+p[2]+'"/></svg>'+p[1]+(p[0]==='nhat-ky'?'<span class="badge" id="navBadge" hidden>0</span>':'')+'</button>'}).join('');
$('#tabbar').innerHTML=PAGES.map(function(p){return '<button type="button" data-go="'+p[0]+'"><svg><use href="#'+p[2]+'"/></svg>'+p[1]+'</button>'}).join('');
function route(){var h=(location.hash||'').slice(1),pg=PAGES.some(function(p){return p[0]===h})?h:'tong-quan';
  $$('section[data-page]').forEach(function(s){s.hidden=s.dataset.page!==pg});
  $$('[data-go]').forEach(function(b){if(b.closest('#nav,#tabbar')){if(b.dataset.go===pg)b.setAttribute('aria-current','page');else b.removeAttribute('aria-current')}});
  $('#title').textContent=PAGES.filter(function(p){return p[0]===pg})[0][1];window.scrollTo(0,0)}
window.addEventListener('hashchange',route);

/* ================= Ý NGHĨA CẢM BIẾN (giống trên LCD) ================= */
function sensors(h,now){var t=h.tempF!=null?h.tempF:h.temp,hu=h.hum,pct=Math.max(0,Math.min(100,100-Math.round(h.light*100/1023)));
  return [
    {k:'therm',n:'Nhiệt độ',v:t.toFixed?Number(t).toFixed(1).replace('.',',')+'°C':t+'°C',w:t>=35?'Rất nóng':t>30?'Hơi nóng':t<20?'Hơi lạnh':'Dễ chịu',lv:t>=35?'bad':t>30?'mid':'ok',say:t>=35?'Uống nhiều nước, bật quạt':t>30?'Nên bật quạt':t<20?'Mặc thêm áo nhẹ':'Thoải mái',g:(t-15)/25},
    {k:'drop',n:'Độ ẩm',v:hu+'%',w:hu>75?'Ẩm ướt':hu<40?'Khô':'Vừa',lv:hu>75?'mid':'ok',say:hu>75?'Phơi đồ trong nhà':hu<40?'Nhớ uống nước':'Không khí dễ chịu',g:hu/100},
    {k:h.dark?'moon':'sun',n:'Ánh sáng',v:pct+'%',w:h.dark?'Trời tối':'Trời sáng',lv:'ok',say:h.dark?(h.ringOn?'Đèn phòng khách đã tự bật':'Nên bật đèn'):'Chưa cần bật đèn',g:pct/100},
    {k:'rain',n:'Mưa',v:h.raining?'Có':'Không',w:h.raining?'Đang mưa':'Khô ráo',lv:h.raining?'bad':'ok',say:h.raining?'Cất quần áo vào!':'Hôm nay mưa '+h.rainCount+' lần'},
    {k:'fire',n:'Lửa',v:h.fire?'CÓ LỬA':'Không',w:h.fire?'Nguy hiểm':'An toàn',lv:h.fire?'bad':'ok',say:h.fire?'Ra ngoài ngay, gọi 114':'Bếp không có lửa'},
    {k:'sound',n:'Âm thanh',v:h.sound?'Có tiếng':'Yên',w:h.sound?'Có tiếng động':'Yên tĩnh',lv:h.sound&&h.mode===1?'bad':'ok',say:'Vỗ tay '+h.clap+' lần hôm nay'}]}
var ICON_OF={therm:'i-therm',drop:'i-drop',sun:'i-sun',moon:'i-moon',rain:'i-rain',fire:'i-fire',sound:'i-sound'};

/* ================= NHẬT KÝ ================= */
var KIND={owner:['👑','Chủ nhà về','acc','door'],ok:['🔓','Mở cửa bằng thẻ','good','door'],remote:['📱','Mở cửa từ xa','acc','door'],grant:['➕','Thêm thẻ','good','door'],revoke:['➖','Thu hồi thẻ','warn','door'],
  unknown:['⚠️','Thẻ lạ quẹt thử','warn','door warn'],locked:['🔒','Khóa đầu đọc 30 giây','crit','door warn'],intrusion:['🚨','Rung mạnh ở cửa','crit','door warn'],silence:['🔕','Tắt báo động','','door'],sys:['🛡️','Khóa thẻ','','door'],
  h_fire:['🔥','Lửa trong nhà','crit','home warn'],h_rain:['🌧️','Mưa','warn','home'],h_hot:['🌡️','Nóng','warn','home'],h_light:['💡','Đèn','','home'],h_mode:['🏠','Chế độ nhà','acc','home'],h_intrude:['🚨','Có kẻ lạ trong nhà','crit','home warn'],h_sys:['⚙️','Trong nhà','','home'],fire_door:['🚪','Cửa thoát hiểm','crit','door warn']};
function fmtT(ms){var d=new Date(ms),n=new Date();return pad(d.getHours())+':'+pad(d.getMinutes())+(d.toDateString()===n.toDateString()?'':' · '+pad(d.getDate())+'/'+pad(d.getMonth()+1))}
function evTitle(e){var K=KIND[e.kind]||['•','Sự kiện','','home'];
  if(e.kind==='ok')return [K[0],esc(e.who)+' vào nhà','Quẹt thẻ · cửa tự đóng sau 5 giây',K[2]];
  if(e.kind==='owner')return [K[0],'Chủ nhà về','Mở cửa và bật chế độ thêm thẻ',K[2]];
  if(e.kind==='remote')return [K[0],'Mở cửa từ xa','Từ '+esc(e.who).toLowerCase(),K[2]];
  if(e.kind==='unknown')return [K[0],'Thẻ lạ quẹt thử','Mã thẻ '+esc(e.uid)+' · cửa vẫn đóng',K[2]];
  if(e.kind==='revoke')return [K[0],'Thu hồi thẻ của '+esc(e.who),esc(e.uid),K[2]];
  return [K[0],K[1],esc(e.who),K[2]]}
function feedHTML(list){if(!list.length)return '<li><span></span><span class="t note">Chưa có gì</span><span></span></li>';
  return list.map(function(e){var t=evTitle(e);return '<li><span class="ic '+t[3]+'" aria-hidden="true">'+t[0]+'</span><span class="t">'+t[1]+'<small>'+t[2]+'</small></span><time>'+fmtT(e.t)+'</time></li>'}).join('')}
function paintLog(){
  $('#feedShort').innerHTML=feedHTML(log.slice(0,6));
  $('#feedDoor').innerHTML=feedHTML(log.filter(function(e){return /^(owner|ok|remote|unknown|locked|intrusion)$/.test(e.kind)}).slice(0,12));
  $('#feedAll').innerHTML=feedHTML(log.filter(function(e){var c=(KIND[e.kind]||['','','','home'])[3];return filt==='all'||c.indexOf(filt)>=0}));
  var w=log.filter(function(e){return /warn/.test((KIND[e.kind]||[])[3]||'')&&Date.now()-e.t<3600000}).length,b=$('#navBadge');b.hidden=!w;b.textContent=w}

/* ================= VẼ ================= */
function paintDoor(d){var now=Date.now();
  var M={IDLE:['Đang canh','good'],READY:['Có người trước cửa','acc'],GRANTED:[d.greet==='Mo tu xa'?'Vừa mở từ xa':'Đang chào khách','good'],OWNER:['Chủ nhà về','acc'],GRANT:['Đang chờ thẻ mới','acc'],
    GRANT_OK:['Vừa thêm thẻ','good'],GRANT_DEL:['Vừa thu hồi thẻ','warn'],DENIED:['Thẻ lạ','warn'],LOCKED:['Khóa 30 giây','warn'],ALARM:['BÁO ĐỘNG','crit'],FIRE:['CHÁY · mở thoát hiểm','crit']}[d.st]||['—',''];
  [$('#doorPill'),$('#doorPill2')].forEach(function(p){p.textContent=M[0];p.className='pill '+M[1]});
  $('#doorBig').firstChild.nodeValue=d.door?'Đang mở':'Đang đóng';
  $('#doorSmall').textContent=d.st==='FIRE'?'Giữ mở để thoát hiểm, tự đóng 1 phút sau khi hết cháy':d.door?'Tự đóng sau vài giây':(PD&&PD.home&&PD.home.lastEntry?'Người vào gần nhất: '+PD.home.lastEntry.name+' lúc '+PD.home.lastEntry.time:'Chưa ai vào hôm nay');
  var sp=$('#sysPill');sp.textContent=d.sys?'Khóa thẻ thành viên':'Thẻ thành viên dùng được';sp.className='pill '+(d.sys?'warn':'good');
  $('#bSys').lastChild.nodeValue=d.sys?'Mở khóa thẻ':'Khóa thẻ';
  $$('#segSys button').forEach(function(b){b.setAttribute('aria-pressed',String((b.dataset.v==='1')===d.sys))});
  $('#fails').textContent=d.fails+' / '+(d.maxf||3);$('#distTxt').textContent=d.dist>0?d.dist+' cm':'Không có ai';
  var vp=$('#vibPill');vp.textContent=d.st==='ALARM'?'Rung mạnh!':'Yên';vp.className='pill '+(d.st==='ALARM'?'crit':'good');
  $('#bSilence').disabled=d.st!=='ALARM';$$('[data-act="open"]').forEach(function(b){b.disabled=d.door||d.st==='ALARM'});
  var g=d.st==='GRANT';$('#grantBox').hidden=!g;if(g){var l=d.left/1000;$('#grantCd').textContent=Math.ceil(l);$('#grantBar').style.width=(l/(d.grantSec||15)*100)+'%'}
  $('#bGrant').lastChild.nodeValue=g?'Đang chờ thẻ…':'Thêm thẻ';
  /* robot */
  S.mode=d.st;S.greet=d.greet||'';S.fails=d.fails;S.sysLock=d.sys;S.until=now+(d.left||0);S.grantUntil=d.st==='GRANT'?now+d.left:0;S.dist=d.dist;if(d.near)S.lastAct=now;
}
function paintHome(h){if(!h)return;var now=Date.now(),L=sensors(h,now);
  var mp=$('#modePill'),MODE=[['Ở nhà','acc'],['Vắng nhà','warn'],['Đi ngủ','']][h.mode]||['Chào mừng','good'];mp.textContent=MODE[0]+(h.mode===1&&!h.armed?' · đang đếm ngược':h.mode===1?' · đang canh gác':'');mp.className='pill '+MODE[1];
  var bad=L.filter(function(s){return s.lv==='bad'});var sum=bad.length?bad.map(function(s){return s.w}).join(' · '):'Mọi thứ đều ổn';
  $('#homeSum').textContent=sum;$('#homeSum2').textContent=sum;
  $('#sensTiles').innerHTML=L.map(function(s){return '<div class="tile '+s.lv+'"><span class="lb"><svg><use href="#'+ICON_OF[s.k]+'"/></svg>'+s.n+'</span><span class="v">'+s.v+'</span><span class="w">'+s.w+'</span></div>'}).join('');
  $('#sensList').innerHTML=L.map(function(s){return '<li><span class="ic"><svg><use href="#'+ICON_OF[s.k]+'"/></svg></span><span><b>'+s.n+' · '+s.w+'</b><small>'+s.say+'</small>'+(s.g!=null?'<span class="gauge"><i style="left:'+Math.max(0,Math.min(100,s.g*100)).toFixed(0)+'%"></i></span>':'')+'</span><span class="val"><b>'+s.v+'</b></span></li>'}).join('');
  $$('[data-hmode]').forEach(function(b){var v=b.dataset.hmode;b.hidden=v==='home'?h.mode===0:h.mode!==0});
  $$('#segMode button').forEach(function(b){b.setAttribute('aria-pressed',String(['home','away','sleep'].indexOf(b.dataset.v)===h.mode))});
  $('#modeNote').textContent=h.mode===0?'Đèn tự động, vỗ tay 2 lần bật đèn hành lang.':h.mode===1?(h.armed?'Có tiếng động trong nhà mà không ai quẹt thẻ là báo kẻ lạ.':'Còn vài giây để ra ngoài.'):'Đèn tắt, màn hình hiện đồng hồ lớn. Báo thức lúc 06:00.';
  /* đèn */
  $$('.seg[data-light] button').forEach(function(b){var cur=b.parentNode.dataset.light==='ring'?h.lightMode:h.hallMode;b.setAttribute('aria-pressed',String(+b.dataset.v===cur))});
  $('#ringTxt').textContent='Phòng khách đang '+(h.ringOn?'bật':'tắt');$('#ringNote').textContent=['Tự bật khi trời tối','Luôn bật','Luôn tắt'][h.lightMode];
  $('#hallTxt').textContent='Đang '+(h.hall?'bật':'tắt')+(h.hallMode===0?' · vỗ tay 2 lần để bật':'');
  $('#ringFx').textContent={0:'Đèn thường',7:'Canh gác'}[h.ringFx]||'Hiệu ứng';
  var r='';for(var i=0;i<12;i++){var a=i/12*Math.PI*2-Math.PI/2,c=h.ring[i]||[0,0,0],on=c[0]+c[1]+c[2]>20;
    r+='<circle cx="'+(42+31*Math.cos(a)).toFixed(1)+'" cy="'+(42+31*Math.sin(a)).toFixed(1)+'" r="6" fill="'+(on?'rgb('+c.join(',')+')':'var(--line)')+'"'+(on?' style="filter:drop-shadow(0 0 4px rgb('+c.join(',')+'))"':'')+'/>'}
  $('#ring').innerHTML='<circle cx="42" cy="42" r="31" fill="none" stroke="var(--line)" stroke-width="10" opacity=".5"/>'+r;
  /* biểu đồ 24 giờ */
  var hs=h.hist24||[],mn=Math.min.apply(null,hs),mx=Math.max.apply(null,hs),lo=Math.floor(mn-1),hi=Math.ceil(mx+1),W=320,Ht=130,x0=28,y0=8,x1=312,y1=104;
  function X(i){return x0+(x1-x0)*i/(hs.length-1)}function Y(v){return y1-(y1-y0)*(v-lo)/(hi-lo)}
  var pts=hs.map(function(v,i){return X(i).toFixed(1)+','+Y(v).toFixed(1)}).join(' '),grid='',hN=new Date().getHours();
  [lo,Math.round((lo+hi)/2),hi].forEach(function(v){grid+='<line x1="'+x0+'" x2="'+x1+'" y1="'+Y(v).toFixed(1)+'" y2="'+Y(v).toFixed(1)+'" stroke="var(--line)" stroke-dasharray="3 4"/><text class="axis" x="'+(x0-6)+'" y="'+(Y(v)+4).toFixed(1)+'" text-anchor="end">'+v+'°</text>'});
  [0,6,12,18,23].forEach(function(i){grid+='<text class="axis" x="'+X(i).toFixed(1)+'" y="'+(Ht-8)+'" text-anchor="middle">'+pad((hN-23+i+24)%24)+'h</text>'});
  var last=hs.length-1;
  $('#tChart').innerHTML='<defs><linearGradient id="tg" x1="0" y1="0" x2="0" y2="1"><stop offset="0" stop-color="var(--accent)" stop-opacity=".28"/><stop offset="1" stop-color="var(--accent)" stop-opacity="0"/></linearGradient></defs>'+grid+
    '<polygon points="'+x0+','+y1+' '+pts+' '+x1+','+y1+'" fill="url(#tg)"/><polyline points="'+pts+'" fill="none" stroke="var(--accent)" stroke-width="2.4" stroke-linejoin="round" stroke-linecap="round"/>'+
    '<circle cx="'+X(last).toFixed(1)+'" cy="'+Y(hs[last]).toFixed(1)+'" r="4.5" fill="var(--panel)" stroke="var(--accent)" stroke-width="2.4"/>';
  $('#kNow').textContent=String(h.tempF!=null?h.tempF:h.temp).replace('.',',')+'°';$('#kMin').textContent=String(mn).replace('.',',')+'°';$('#kMax').textContent=String(mx).replace('.',',')+'°';
  $('#lcdPage').textContent=h.pageName||'';
  drawLcd($('#lcd'),h.lcd);drawLcd($('#lcd2'),h.lcd);
}
function drawLcd(cv,l){var cx=cv.getContext('2d'),bl=l&&l.bl;cx.fillStyle=bl?'#2b63c9':'#0d1f3d';cx.fillRect(0,0,400,84);if(!l)return;
  for(var r=0;r<2;r++)for(var c=0;c<16;c++){var g=lcdGlyph(l.rows[r][c],l.cg),x0=10+c*24,y0=8+r*38;
    for(var y=0;y<8;y++)for(var x=0;x<5;x++){var on=(g[y]>>(4-x))&1;cx.fillStyle=on?(bl?'#eef6ff':'#5f7ba3'):(bl?'rgba(255,255,255,.07)':'rgba(255,255,255,.03)');cx.fillRect(x0+x*4,y0+y*4,3,3)}}}
function paintTop(d){
  var dw=$('#dWifi');dw.className='dot'+(d.wifi?(d.rssi<-75?' warn':''):' crit');$('#tWifi').textContent=d.wifi?(d.rssi<-75?'WiFi yếu':'WiFi tốt'):'Mất WiFi';
  var dl=$('#dLink');dl.className='dot'+(d.link?'':' crit');$('#tLink').textContent=d.link?'Nhà đã nối':'Mất nối với nhà';
  $('#dTg').className='dot'+(d.tg?'':' warn');$('#fIp').textContent=d.ip||'—';
  var up=Math.floor(d.up/60000);$('#fUp').textContent=Math.floor(up/60)+' giờ '+(up%60)+' phút';
  var hr=new Date().getHours();$('#greet').textContent=(hr<11?'Chào buổi sáng':hr<13?'Chào buổi trưa':hr<18?'Chào buổi chiều':'Chào buổi tối')+'. '+(d.home?(d.home.mode===1?'Nhà đang canh gác.':d.home.mode===2?'Cả nhà đang ngủ.':'Cửa đóng, trong nhà '+(d.home.fire||d.home.raining?'có việc cần xem.':'mọi thứ đều ổn.')):'');
  var a=d.st==='ALARM',f=d.home&&d.home.fire;$('#alarmBar').hidden=!(a||f);
  if(a){$('#alarmT').textContent='🚨 Rung mạnh ở cửa trước';$('#alarmS').textContent='Cửa đã đóng, còi đang kêu. Kiểm tra rồi tắt báo động.'}
  if(f){$('#alarmT').textContent='🔥 Phát hiện lửa trong nhà';$('#alarmS').textContent='Cảm biến lửa ở bếp. Còi trong nhà đang kêu. '+(d.st==='FIRE'||d.door?'Cửa đã tự mở để thoát hiểm.':d.home.mode===1?'Nhà vắng người: cửa giữ đóng, đã báo Telegram.':'')}
}
var GLY_AR=[[0,14,31,31,31,14,0,0],[0,0,0,0,31,0,0,0],[0,0,0,16,8,7,0,0],[0,0,0,1,2,28,0,0],[0,21,14,27,14,21,0,0],[4,0,14,31,14,0,4,0],[4,10,10,10,14,31,31,14],[4,4,10,10,17,17,17,14],[12,6,3,3,3,6,12,0],[0,12,6,3,3,3,6,12],[31,17,17,19,17,17,31,0],[31,19,19,19,19,19,31,0],[14,17,17,31,27,27,31,0],[14,17,1,31,27,27,31,0],[4,14,31,17,21,17,31,0],[14,17,17,17,10,14,14,4],[14,31,31,31,14,14,14,4],[0,4,2,31,2,4,0,0],[14,14,4,14,21,4,10,17],[14,14,4,14,21,4,10,10],[0,0,14,31,31,14,0,0],[14,31,31,0,8,2,8,2],[14,31,31,0,2,8,2,8],[4,12,14,30,31,31,31,14],[8,12,6,15,31,31,31,14],[2,6,14,15,31,31,31,14],[0,1,3,22,28,8,0,0],[4,4,4,4,4,0,4,0],[0,17,10,4,10,17,0,0],[0,10,31,31,14,4,0,0],[4,14,14,14,31,0,4,0],[4,14,14,31,31,0,4,0],[0,0,0,0,16,8,0,0],[0,0,0,16,24,8,0,0],[0,0,1,18,28,8,0,0],[0,16,16,16,16,16,16,0],[0,24,24,24,24,24,24,0],[0,28,28,28,28,28,28,0],[0,30,30,30,30,30,30,0],[0,31,31,31,31,31,31,0],[7,15,31,31,31,31,31,31],[31,31,31,0,0,0,0,0],[28,30,31,31,31,31,31,31],[31,31,31,31,31,31,15,7],[0,0,0,0,0,31,31,31],[31,31,31,31,31,31,30,28],[31,31,31,0,0,0,31,31],[31,0,0,0,0,31,31,31]];
function apply(d){var now=Date.now();PD=d;H=d.home||null;if(H&&H.lcd&&H.lcd.g&&!H.lcd.cg)H.lcd.cg=H.lcd.g.map(function(i){return GLY_AR[i]||[0,0,0,0,0,0,0,0]});S.servo=d.servo!=null?d.servo:(d.door?90:0);if(d.door&&!S.door)S.lastOpen=now;S.door=!!d.door;
  (d.ev||[]).forEach(function(e){if(e.id<=since)return;log.unshift({id:e.id,t:now-e.age,kind:e.k,uid:e.u,who:e.w});since=e.id});
  log.sort(function(a,b){return b.t-a.t});
  if(d.cards){cards=d.cards;owner=d.owner;cardsVer=d.cv;paintCards()}
  paintTop(d);paintDoor(d);paintHome(d.home);paintLog();sceneDoorText();
}
/* ================= THẺ ================= */
var editing=-1,askDel=-1;
function paintCards(){var L=$('#cardList'),html='';
  if(owner)html+='<li><span class="av own">👑</span><span><b>'+esc(owner.n==='Chu nha'?'Chủ nhà':owner.n)+'</b><small>'+esc(owner.u)+' · mở cửa và thêm thẻ</small></span><span></span></li>';
  cards.forEach(function(c){
    var nm=esc(c.n),av=esc(ascii(c.n).charAt(0).toUpperCase()||'?');
    if(editing===c.i)html+='<li><span class="av">'+av+'</span><span><input id="rn'+c.i+'" value="'+nm+'" maxlength="16" aria-label="Tên mới"><small>'+esc(c.u)+'</small></span><span class="ops"><button class="btn pri" type="button" data-card="save" data-i="'+c.i+'">Lưu</button><button class="btn" type="button" data-card="cancel">Thôi</button></span></li>';
    else if(askDel===c.i)html+='<li><span class="av">'+av+'</span><span><b>Thu hồi thẻ của '+nm+'?</b><small>Thẻ này sẽ không mở được cửa nữa</small></span><span class="ops"><button class="btn danger" type="button" data-card="delYes" data-i="'+c.i+'">Thu hồi</button><button class="btn" type="button" data-card="cancel">Thôi</button></span></li>';
    else html+='<li><span class="av">'+av+'</span><span><b>'+nm+'</b><small>'+esc(c.u)+'</small></span><span class="ops"><button class="btn" type="button" data-card="edit" data-i="'+c.i+'">Đổi tên</button><button class="btn danger" type="button" data-card="del" data-i="'+c.i+'">Thu hồi</button></span></li>'});
  L.innerHTML=html;$('#cardCount').textContent=cards.length+' thành viên + chủ nhà';
  if(editing>=0){var inp=$('#rn'+editing);if(inp){inp.focus();inp.select()}}}

/* ================= THAO TÁC ================= */
var tt=null;function toast(m){var t=$('#toast');t.textContent=m;t.classList.add('on');clearTimeout(tt);tt=setTimeout(function(){t.classList.remove('on')},2400)}
function call(path){return api(path).then(function(j){toast(j.msg||(j.ok?'Xong':'Không làm được'));poll(true);return j},function(){toast('Không kết nối được ESP32');return {ok:false}})}
document.addEventListener('click',function(e){
  var g=e.target.closest('[data-go]');if(g){location.hash=g.dataset.go;return}
  var a=e.target.closest('[data-act]');
  if(a){var w=a.dataset.act;
    if(w==='open'){$('#openAsk').hidden=false;if(!$('section[data-page="tong-quan"]').hidden)return;call('/api/act?do=open');return}
    if(w==='openDirect'){call('/api/act?do=open');return}
    if(w==='openYes'){$('#openAsk').hidden=true;call('/api/act?do=open');return}
    if(w==='openNo'){$('#openAsk').hidden=true;return}
    if(w==='syslock'){call('/api/act?do='+(PD&&PD.sys?'unlock':'lock'));return}
    if(w==='grant'&&PD&&PD.st==='GRANT'){location.hash='the';return}
    if(w==='grant'){call('/api/act?do=grant');location.hash='the';return}
    call('/api/act?do='+w);return}
  var s=e.target.closest('#segSys button');if(s){call('/api/act?do='+(s.dataset.v==='1'?'lock':'unlock'));return}
  var hm=e.target.closest('[data-hmode],#segMode button');if(hm){call('/api/home?do=mode&v='+(hm.dataset.hmode||hm.dataset.v));return}
  var lb=e.target.closest('.seg[data-light] button');if(lb){call('/api/home?do='+(lb.parentNode.dataset.light==='ring'?'light':'hall')+'&v='+lb.dataset.v);return}
  var c=e.target.closest('[data-card]');if(c){var k=c.dataset.card,i=+c.dataset.i;
    if(k==='edit'){editing=i;askDel=-1;paintCards()}else if(k==='del'){askDel=i;editing=-1;paintCards()}else if(k==='cancel'){editing=askDel=-1;paintCards()}
    else if(k==='save'){var v=$('#rn'+i).value.trim();editing=-1;if(v)call('/api/card?do=rename&i='+i+'&n='+encodeURIComponent(v));else paintCards()}
    else if(k==='delYes'){askDel=-1;call('/api/card?do=del&i='+i)}return}
  var f=e.target.closest('#filters button');if(f){filt=f.dataset.f;$$('#filters button').forEach(function(b){b.setAttribute('aria-pressed',String(b===f))});paintLog()}
});
document.addEventListener('keydown',function(e){if(e.key==='Enter'&&e.target.id&&e.target.id.indexOf('rn')===0){var i=+e.target.id.slice(2);var b=$('[data-card="save"][data-i="'+i+'"]');if(b)b.click()}if(e.key==='Escape'&&(editing>=0||askDel>=0)){editing=askDel=-1;paintCards()}});

/* ================= VÒNG CẬP NHẬT ================= */
var busy=false;
function poll(now){if(busy&&!now)return;busy=true;api('/api/poll?since='+since+'&cv='+cardsVer).then(function(d){apply(d)}).catch(function(){}).then(function(){busy=false})}
setInterval(poll,700);poll();
setInterval(function(){var d=new Date();$('#clock').textContent=pad(d.getHours())+':'+pad(d.getMinutes())+':'+pad(d.getSeconds())},500);
function frame(t){drawRobot(t);if(!$('section[data-page="ngoi-nha"]').hidden)sceneFrame(t);requestAnimationFrame(frame)}requestAnimationFrame(frame);
route();
})();
</script>

</body></html>
)rawliteral";
