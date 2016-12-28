package server;

//Tiago Coelho: enable cross-origin requests workaround by spring!

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.ResourceHandlerRegistry;
import org.springframework.web.servlet.config.annotation.ViewControllerRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurerAdapter;

@Configuration
public class StaticResourceConfiguration extends WebMvcConfigurerAdapter {

 @Value("${static.path}")
 private String staticPath;

 @Override
 public void addResourceHandlers(ResourceHandlerRegistry registry) {

    if(staticPath != null) {
        System.out.println("Serving static content from " + staticPath);
        registry.addResourceHandler("/**").addResourceLocations("file:" + staticPath);
    }
 }

 @Override
 public void addViewControllers(ViewControllerRegistry registry) {
    registry.addViewController("/").setViewName("redirect:/index.html");
 }
}