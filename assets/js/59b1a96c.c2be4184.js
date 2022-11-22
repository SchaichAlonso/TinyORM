"use strict";(self.webpackChunktinyorm_org=self.webpackChunktinyorm_org||[]).push([[67],{3905:(e,t,a)=>{a.d(t,{Zo:()=>d,kt:()=>c});var r=a(7294);function n(e,t,a){return t in e?Object.defineProperty(e,t,{value:a,enumerable:!0,configurable:!0,writable:!0}):e[t]=a,e}function i(e,t){var a=Object.keys(e);if(Object.getOwnPropertySymbols){var r=Object.getOwnPropertySymbols(e);t&&(r=r.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),a.push.apply(a,r)}return a}function o(e){for(var t=1;t<arguments.length;t++){var a=null!=arguments[t]?arguments[t]:{};t%2?i(Object(a),!0).forEach((function(t){n(e,t,a[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(a)):i(Object(a)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(a,t))}))}return e}function l(e,t){if(null==e)return{};var a,r,n=function(e,t){if(null==e)return{};var a,r,n={},i=Object.keys(e);for(r=0;r<i.length;r++)a=i[r],t.indexOf(a)>=0||(n[a]=e[a]);return n}(e,t);if(Object.getOwnPropertySymbols){var i=Object.getOwnPropertySymbols(e);for(r=0;r<i.length;r++)a=i[r],t.indexOf(a)>=0||Object.prototype.propertyIsEnumerable.call(e,a)&&(n[a]=e[a])}return n}var s=r.createContext({}),p=function(e){var t=r.useContext(s),a=t;return e&&(a="function"==typeof e?e(t):o(o({},t),e)),a},d=function(e){var t=p(e.components);return r.createElement(s.Provider,{value:t},e.children)},u={inlineCode:"code",wrapper:function(e){var t=e.children;return r.createElement(r.Fragment,{},t)}},m=r.forwardRef((function(e,t){var a=e.components,n=e.mdxType,i=e.originalType,s=e.parentName,d=l(e,["components","mdxType","originalType","parentName"]),m=p(a),c=n,y=m["".concat(s,".").concat(c)]||m[c]||u[c]||i;return a?r.createElement(y,o(o({ref:t},d),{},{components:a})):r.createElement(y,o({ref:t},d))}));function c(e,t){var a=arguments,n=t&&t.mdxType;if("string"==typeof e||n){var i=a.length,o=new Array(i);o[0]=m;var l={};for(var s in t)hasOwnProperty.call(t,s)&&(l[s]=t[s]);l.originalType=e,l.mdxType="string"==typeof e?e:n,o[1]=l;for(var p=2;p<i;p++)o[p]=a[p];return r.createElement.apply(null,o)}return r.createElement.apply(null,a)}m.displayName="MDXCreateElement"},2075:(e,t,a)=>{a.r(t),a.d(t,{assets:()=>s,contentTitle:()=>o,default:()=>u,frontMatter:()=>i,metadata:()=>l,toc:()=>p});var r=a(7462),n=(a(7294),a(3905));const i={sidebar_position:0,sidebar_label:"\ud83d\udd25 Prologue",slug:"/",hide_table_of_contents:!0,description:"TinyORM is a modern c++ ORM library that makes interacting with a database extremely simple. It depends on the QtCore and QtSql libraries. The code is written in the modern c++20 way and is well tested with the unit and functional tests.",keywords:["c++ orm","prologue","tinyorm"]},o="Prologue",l={unversionedId:"README",id:"README",title:"Prologue",description:"TinyORM is a modern c++ ORM library that makes interacting with a database extremely simple. It depends on the QtCore and QtSql libraries. The code is written in the modern c++20 way and is well tested with the unit and functional tests.",source:"@site/docs/README.mdx",sourceDirName:".",slug:"/",permalink:"/",draft:!1,editUrl:"https://github.com/silverqx/TinyORM-github.io/edit/main/docs/README.mdx",tags:[],version:"current",sidebarPosition:0,frontMatter:{sidebar_position:0,sidebar_label:"\ud83d\udd25 Prologue",slug:"/",hide_table_of_contents:!0,description:"TinyORM is a modern c++ ORM library that makes interacting with a database extremely simple. It depends on the QtCore and QtSql libraries. The code is written in the modern c++20 way and is well tested with the unit and functional tests.",keywords:["c++ orm","prologue","tinyorm"]},sidebar:"tinyormSidebar",next:{title:"\ud83d\udd27 Dependencies",permalink:"/dependencies"}},s={},p=[{value:"Documentation Sitemap",id:"documentation-sitemap",level:5},{value:"Current versions",id:"current-versions",level:5}],d={toc:p};function u(e){let{components:t,...a}=e;return(0,n.kt)("wrapper",(0,r.Z)({},d,a,{components:t,mdxType:"MDXLayout"}),(0,n.kt)("h1",{id:"prologue"},"Prologue"),(0,n.kt)("p",null,"TinyORM is a modern c++ ORM library that makes interacting with a database extremely simple. It depends on the ",(0,n.kt)("inlineCode",{parentName:"p"},"QtCore")," and ",(0,n.kt)("inlineCode",{parentName:"p"},"QtSql")," libraries."),(0,n.kt)("p",null,"The code is written in the modern c++20 way and is well tested with the unit and functional tests. Almost all the query builder methods are unit tested. The TinyORM's query builder code and the code which is responsible for obtaining relationships, is tested by functional tests against all supported databases. The code coverage is good enough to guarantee API and behavior compatibility."),(0,n.kt)("admonition",{type:"tip"},(0,n.kt)("p",{parentName:"admonition"},"For a quick look at what's inside, check out the ",(0,n.kt)("a",{parentName:"p",href:"/features-summary"},"Features Summary"),".")),(0,n.kt)("admonition",{type:"info"},(0,n.kt)("p",{parentName:"admonition"},"If you don't want to use full ",(0,n.kt)("a",{parentName:"p",href:"/tinyorm/getting-started"},(0,n.kt)("inlineCode",{parentName:"a"},"ORM")),", then you can use only the ",(0,n.kt)("a",{parentName:"p",href:"/database/query-builder"},(0,n.kt)("inlineCode",{parentName:"a"},"Query Builder")),", which is outstanding. \ud83d\udd25 This way you can avoid writing raw SQL queries and your code will run on all ",(0,n.kt)("a",{parentName:"p",href:"/database/getting-started#introduction"},"supported databases"),".")),(0,n.kt)("h5",{id:"documentation-sitemap"},"Documentation Sitemap"),(0,n.kt)("ul",null,(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/dependencies"},"Dependencies")),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/supported-compilers"},"Supported Compilers")),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/database/getting-started"},"Database"),(0,n.kt)("ul",{parentName:"li"},(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/database/getting-started"},"Getting Started")),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/database/query-builder"},"Query Builder")),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/database/migrations"},"Migrations")),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/database/seeding"},"Seeding")))),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/tinyorm/getting-started"},"TinyORM"),(0,n.kt)("ul",{parentName:"li"},(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/tinyorm/getting-started"},"Getting Started")),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/tinyorm/relationships"},"Relationships")),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/tinyorm/casts"},"Casts")))),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/building/tinyorm"},"Building"),(0,n.kt)("ul",{parentName:"li"},(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/building/tinyorm"},"TinyORM")),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/building/hello-world"},"Hello world")),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/building/migrations"},"Migrations")))),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("a",{parentName:"li",href:"/features-summary"},"Features Summary"))),(0,n.kt)("h5",{id:"current-versions"},"Current versions"),(0,n.kt)("ul",null,(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("strong",{parentName:"li"},"TinyORM")," v0.23.1"),(0,n.kt)("li",{parentName:"ul"},(0,n.kt)("strong",{parentName:"li"},"tom")," v0.4.2")))}u.isMDXComponent=!0}}]);