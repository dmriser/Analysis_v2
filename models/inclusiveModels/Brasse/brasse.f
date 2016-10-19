      real function brasse(e,q2,w1)

      include 'brasse.inc'
      
      logical goodfit
      
      real e,q2,q2r,w1,dsig
      
      x = q2/(w1**2-0.93828**2+q2)
      q2r = q2
      
c     call r1998(x,q2r,rc,dr,goodfit)
      
c      if (.not.goodfit) rc=0.25
c      print *, r
      rc=0.15
      r = rc
     
      call brasse_sub(e,q2,w1,dsig)
      brasse = dsig

      end
        
        subroutine brasse_sub(e,qs,w1,dsig)
        
c       calculate inclusive inelastic cross section using parameters
c       of Brasse et al.
c       units of photon absorption cross section are microbarns
c       Brasse's cross section is for absorption of the virtual photon
c       It must be multiplied by the photon flux factor.

        implicit none
        
c       Brasse parametrizes the cross section at each resonance mass
c       w in terms of 3 parameters, a,b,c.  They are almost independent
c       of epsilon.        
c
c       For W>2GeV, use a simple parametrization based on F2

        include 'brasse.inc'

        real w0,a0,b0,c0,qs,qvecs,a1,b1,c1
        real qvec,qvec0,sig,fdip,epsi,epsinv
        real nu,e,epr,flux,snth2,csth2,tnth2,ctnth2,dsig
        real jacob
        real sig0,sig1
        real w1,w2,deltaw

        integer i,iw

c       Calculate the proton dipole form factor that Brasse divided out
c       of the data before doing the fits. 
       
        fdip=1/(1+qs/.707)**2
        fdip=fdip**2
        
        iw=1
        do i=1,nsteps-2
           if (w(i) .gt. w1)then
              iw=i-1
              go to 14
           endif
        enddo
 14     continue

        i=iw
        w0=w(i)
        deltaw=(w1-w0)/(w(i+1)-w0)
        
c       calculate kinematical quantities
        w2 = w0*w0
        nu=(w2-mp**2+qs)/2/mp
        epr=e-nu
        if (epr .lt. 0.05) go to 85

c       calculate the scattering angle

        snth2=qs/4/e/epr

        if (snth2 .gt. 1.)go to 85
        csth2=1-snth2
        tnth2=snth2/csth2
        ctnth2=1.0/tnth2                  

        qvecs=qs+nu**2
        qvec=sqrt(qvecs)
        qvec0=(w2-mp**2)/2/mp

c       calculate the polarization factor 

        epsinv=1+2*(1+nu**2/qs)*tnth2
        epsi=1/epsinv    
        
        if (i .ge. nsteps/4-2) go to 30

        a0=a(i)
        b0=b(i)
        c0=c(i)
        a1=a(i+1)
        b1=b(i+1)
        c1=c(i+1)

        sig0=a0+b0*alog(qvec/qvec0)+c0*(alog(qvec/qvec0))**3
        sig1=a1+b1*alog(qvec/qvec0)+c1*(alog(qvec/qvec0))**3

        sig=exp(sig0+deltaw*(sig1-sig0))*fdip
        sig=sig*(1+epsi*R)/(1+0.96*R)

c       calculate the flux factor to obtain a differential cross
c       section in angle and scattered energy:

        flux=alpha/2/pi**2
        flux=flux*(w2-mp**2)/2/mp/qs
        flux=flux*epr/e/(1-epsi) 
        
c       calculate dsigma/d E'/d Omega
*        dsig=sig
        dsig=sig*flux
*        return
        go to 40
        
c       Use a simple F2 parametrization above the resonance region

30      call xsecf2(e,qs,nu,epsi,snth2,dsig)

40      continue 
        jacob=w0/2/mp/e/epr
c       integrate over phi

        jacob=2*pi*jacob
        dsig=dsig*jacob 

        return

 85     dsig=0.
 
        return
        end
        
        subroutine xsecf2(e,q2,nu,epsi,snth2,dsig)

        include 'brasse.inc'

        real e,q2,nu,epsi,dsig,snth2
        real s,x,w2,xmott

        s=mp**2+2*mp*e
        xmott=4*(e-nu)**2*(alpha/q2)**2*(1-snth2)
        dsig=(1+epsi*R)/epsi/(1+R)*xmott
        x=q2/2/mp/nu
        W2=.42*(1-x/0.93)/nu
        dsig=(19.7327**2)*dsig*W2
        return
        end
        
        subroutine input(iunit)
        implicit none
        
        include 'brasse.inc'

        real w0
        integer i,iunit

        mp=0.93828
        alpha=1/137.01
        pi=3.14159

c       The parameters are in the file brasse.inp

c       Use a constant R=sigma_L\sigma_T of 0.1.  The
c       parametrization for sig is at epsilon=0.96.

        R=0.1
        delw=0.015
        open(unit = iunit, file = "brasse.inp")
        print *, 'Inputing Brasse parameters'
        
        do i=1,100
	  read(iunit,*,err=10,end=10)w(i),a(i),b(i),c(i)
        enddo
        
        close(10)
        
10      nsteps=i-1
        close(iunit)

c       Triple the number of W increments

        w0=w(nsteps)

        do i=1,3*nsteps
          w0=w0+delw
          w(i+nsteps)=w0
        enddo
        nsteps=4*nsteps

        return
        end
      
                                                                              
      SUBROUTINE R1998(X,Q2,R,DR,GOODFIT)                               
                                                                        
c----------------------------------------------------------------       
c X      : Bjorken x                                                    
c Q2     : Q squared in (GeV/c)**2                                      
c R      :                                                              
c DR     : Absolute error on R                                          
c GOODFIT:  = .TRUE. if the X,Q2 are within the range of the fit.       
c-------------------------------------------------------------------    
c Model for R, based on a fit to world R measurements. Fit performed by 
c program RFIT8 in pseudo-gaussian variable: log(1+.5R).  For details   
c see Reference.                                                        
c                                                                       
c Three models are used, each model has three free parameters.  The     
c functional forms of the models are phenomenological and somewhat      
c contrived.  Each model fits the data very well, and the average of    
c the fits is returned.  The standard deviation of the fit values is    
c used to estimate the systematic uncertainty due to model dependence.  
c                                                                       
c Statistical uncertainties due to fluctuations in measured values have 
c have been studied extensively.  A parametrization of the statistical  
c uncertainty of R1990 is presented in FUNCTION DR1990.                 
c                                                                       
c                                                                       
c Each model fits very well.  As each model has its own strong points   
c and drawbacks, R1998 returns the average of the models.  The          
c chisquare for each fit (237 points with 6 parameters) are:  
c          ALL DATA  #PTS=237         |     X<  0.07 #PTS= 28
c FIT  #PARAM CHISQ  CHISQ/DF PROB(%) |  CHISQ  CHISQ/DF   PROB(%)
c R1998         217.4   0.94    73.1        28.7   1.06    37.5
c R1998a   6    219.4   0.95    69.8        28.8   1.07    37.1
c R1998b   6    217.7   0.94    72.6        27.8   1.03    42.2
c R1998c   6    221.9   0.96    65.5        30.9   1.15    27.4                

c                                                                       
c This subroutine returns reasonable values for R for all x and for all 
c Q2 greater than or equal to .3 GeV.                                   
c                                                                       
c The uncertainty in R originates in three sources:                     
c                                                                       
c     D1 = uncertainty in R due to statistical fluctuations of the data 
c          as reflected in the error of the fit. 
c          It is parameterized in FUNCTION DR1990, for details see     
c          Reference.                                                   
c                                                                       
c     D2 = uncertainty in R due to possible model dependence, approxi-  
c          mated by the variance between the models.                    
c                                                                       
c     D3 = uncertainty in R due to possible epsilon dependent errors    
c          in the radiative corrections, taken to be +/- .025.  See     
c          theses (mine or Dasu's) for details.  This is copied from R1990      
                 
c                                                                       
c and the total error is returned by the program:                       
c                                                                       
c     DR = is the total uncertainty in R, DR = sqrt(D1ESC2+D2ESC2+D3ESC2). 670
c          DR is my best estimate of how well we have measured R.  At   
c          high Q2, where R is small, DR is typically larger than R.  If
c          you have faith in QCD, then, since R1990 = Rqcd at high Q2,  
c          you might wish to assume DR = 0 at very high Q2.

                          
c NOTE:    In many applications, for example the extraction of F2 from  
c          measured cross section, you do not want the full error in R  
c          given by DR.  Rather, you will want to use only the D1 and D2
c          contributions, and the D3 contribution from radiative        
c          corrections propogates complexely into F2.  For more informa-
c          tion, see the documentation to dFRC in HELP.DOCUMENT, or     
c          for explicite detail, see Reference.                         
c                                                                       
c    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
      IMPLICIT NONE                                                     
      REAL QP2,FAC,RLOG,Q2THR,R_A,R_B,R_C,R, D1,D2,D3,DR,DR1998,X,Q2
      REAL A(6),B(6),C(6)

      DATA A /4.8520e-02,  5.4704e-01,  2.0621e+00,
     1       -3.8036e-01,  5.0896e-01, -2.8548e-02/
      DATA B /4.8051e-02,  6.1130e-01, -3.5081e-01, 
     1       -4.6076e-01,  7.1697e-01, -3.1726e-02/
      DATA C /5.7654e-02,  4.6441e-01,  1.8288e+00,
     1        1.2371e+01, -4.3104e+01,  4.1741e+01/


      LOGICAL GOODFIT                                                   
c    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                                 
      FAC = 1.e0+12.e0*Q2/(Q2+1.e0)*(.125e0**2 /(.125e0**2+X**2))
      RLOG  = FAC/LOG(Q2/.04e0) !   <--- we use natural logarithms only!      


c	Model A

      QP2  = (A(2)/(Q2**4+A(3)**4)**.25) * (1.e0+A(4)*X +A(5)*X**2) !1.07 .030
      R_A  = A(1)*RLOG +QP2*X**A(6)   
                                              
c	Model B

      QP2  = (1.e0+B(4)*X+B(5)*X**2)*(B(2)/Q2 +B(3)/(Q2**2+.09e0))  !1.06 .042
      R_B  =  B(1)*RLOG+QP2*X**B(6)  
       
c	Model C

      Q2thr= C(4)*(X) +c(5)*x**2 +c(6)*X**3  
      QP2  = C(2)/SQRT((Q2-Q2thr)**2+C(3)**2) 
      R_C  = C(1)*RLOG+QP2    

      R     = (R_A+R_B+R_C)/3.e0                                          

      D1    = DR1998(X,Q2)                                              

      D2    = SQRT(((R_A-R)**2+(R_B-R)**2+(R_C-R)**2)/2.e0)               
      D3    = .023e0*(1.e0+.5e0*R)                                            

      IF (Q2.LT.1.e0.OR.X.LT..1e0) D3 = 1.5e0*D3                       

      DR    = SQRT(D1**2+D2**2+D3**2)                                   
                                                                        
      GOODFIT = .TRUE.                                                  
      IF ((X.LT.0.02e0).OR.(Q2.LT.0.3e0)) GOODFIT = .FALSE. 
                   
      RETURN                                                            
      END                                                               
                                                                                                
      REAL FUNCTION DR1998(X,Q2)                                             
                                                                        
c Parameterizes the uncertainty in R1990 due to the statistical         
c fluctuations in the data.  Values reflect an average of the R-values  
c about a neighborhood of the specific (x,Q2) value.  That neighborhood 
c is of size [+/-.05] in x, and [+/-33%] in Q2.  For details, see       
c Reference.                                                            
c                                                                       
c This subroutine is accurate over all (x,Q2), not only the SLAC deep   
c inelastic range.  Where there is no data, for example in the resonance
c region, it returns a realistic uncertainty, extrapolated from the deep
c inelastic region (suitably enlarged).  We similarly estimate the      
c uncertainty at very large Q2 by extrapolating from the highest Q2     
c measurments.  For extremely large Q2, R is expected to fall to zero,  
c so the uncertainty in R should not continue to grow.  For this reason 
c DR1990 uses the value at 64 GeV for all larger Q2.                    
c                                                                       
c XHIGH accounts for the rapidly diminishing statistical accuracy for   
c x>.8, and does not contribute for smaller x.                          
                                                                        
                                                                        
      IMPLICIT NONE                                                     
      REAL Q2,X                   

      DR1998 = .0078e0-.013e0*X +(.070e0-.39e0*X+.70e0*X**2)/(1.7e0+Q2)
      RETURN                                                            
      END                                                               
  
  
